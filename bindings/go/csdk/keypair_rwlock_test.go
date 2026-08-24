package csdk

import (
	"bytes"
	"crypto/rand"
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"math/big"
	"sync"
	"sync/atomic"
	"testing"
	"time"

	"github.com/tjfoc/gmsm/sm2"
)

func mustPriv(t *testing.T) []byte {
	t.Helper()
	priv := make([]byte, 32)
	if _, err := rand.Read(priv); err != nil {
		t.Fatalf("rand: %v", err)
	}
	priv[0] &= 0x7f
	if bytes.Equal(priv, make([]byte, 32)) {
		priv[31] = 1
	}
	return priv
}

func newTestSDK(t *testing.T, priv []byte) *CSDK {
	t.Helper()
	sdk, err := NewSignOnlyCSDK(true, priv, "group0", "chain0")
	if err != nil {
		t.Fatalf("NewSignOnlyCSDK: %v", err)
	}
	t.Cleanup(sdk.CloseSignOnly)
	return sdk
}

func hashForIndex(i int) []byte {
	h := make([]byte, 32)
	binary.BigEndian.PutUint64(h[24:], uint64(i+1))
	return h
}

func sm2PrivFromBytes(t *testing.T, priv []byte) *sm2.PrivateKey {
	t.Helper()
	key, err := sm2.GenerateKey(rand.Reader)
	if err != nil {
		t.Fatalf("GenerateKey template: %v", err)
	}
	key.D = new(big.Int).SetBytes(priv)
	key.PublicKey.X, key.PublicKey.Y = key.Curve.ScalarBaseMult(priv)
	return key
}

// verifyFISCOSM2Sig verifies a FISCO SM2 signature (r||s[+pubkey...]) over a 32-byte hash.
func verifyFISCOSM2Sig(t *testing.T, priv []byte, hash, sig []byte) bool {
	t.Helper()
	if len(sig) < 64 {
		t.Fatalf("signature too short: %d", len(sig))
	}
	r := new(big.Int).SetBytes(sig[:32])
	s := new(big.Int).SetBytes(sig[32:64])
	key := sm2PrivFromBytes(t, priv)
	// CreateEncodedSignature signs the raw 32-byte hash as SM2 message (default UID).
	return sm2.Sm2Verify(&key.PublicKey, hash, nil, r, s)
}

func TestConcurrentSameKeySignAndVerify(t *testing.T) {
	priv := mustPriv(t)
	sdk := newTestSDK(t, priv)

	addr, err := sdk.KeyPairAddress()
	if err != nil || addr == "" {
		t.Fatalf("KeyPairAddress: %q err=%v", addr, err)
	}
	pub, err := sdk.KeyPairPublicKey()
	if err != nil || pub == "" {
		t.Fatalf("KeyPairPublicKey: %q err=%v", pub, err)
	}
	if _, err := hex.DecodeString(pub); err != nil {
		t.Fatalf("pub hex: %v", err)
	}

	const n = 128
	var wg sync.WaitGroup
	errs := make(chan error, n)
	wg.Add(n)
	for i := 0; i < n; i++ {
		i := i
		go func() {
			defer wg.Done()
			h := hashForIndex(i)
			sig, err := sdk.CreateEncodedSignature(h)
			if err != nil {
				errs <- fmt.Errorf("sign[%d]: %w", i, err)
				return
			}
			sigCopy := append([]byte(nil), sig...)
			if !verifyFISCOSM2Sig(t, priv, h, sigCopy) {
				errs <- fmt.Errorf("verify[%d]: sm2.Verify failed (sigLen=%d)", i, len(sigCopy))
			}
		}()
	}
	wg.Wait()
	close(errs)
	for err := range errs {
		t.Error(err)
	}
}

func TestConcurrentFullFieldsSignDistinctHashes(t *testing.T) {
	priv := mustPriv(t)
	sdk := newTestSDK(t, priv)

	const n = 64
	results := make([]*SignedTxPair, n)
	var wg sync.WaitGroup
	errs := make(chan error, n)
	wg.Add(n)
	for i := 0; i < n; i++ {
		i := i
		go func() {
			defer wg.Done()
			input := make([]byte, 16)
			binary.BigEndian.PutUint64(input, uint64(i+1))
			nonce := fmt.Sprintf("nonce-%d-%d", i, time.Now().UnixNano())
			pair, err := sdk.CreateSignedTransactionWithDefaultKeyPair(
				1_000_000, "1111111111111111111111111111111111111111", nonce, input, "", 0, "",
			)
			if err != nil {
				errs <- fmt.Errorf("sign[%d]: %w", i, err)
				return
			}
			if pair == nil || pair.TxHash == "" || len(pair.SignedTx) == 0 {
				errs <- fmt.Errorf("sign[%d]: empty result", i)
				return
			}
			results[i] = pair
		}()
	}
	wg.Wait()
	close(errs)
	for err := range errs {
		t.Error(err)
	}

	seen := make(map[string]int, n)
	for i, pair := range results {
		if pair == nil {
			continue
		}
		if prev, ok := seen[pair.TxHash]; ok {
			t.Fatalf("duplicate TxHash for %d and %d: %s", prev, i, pair.TxHash)
		}
		seen[pair.TxHash] = i
	}
}

func TestWriteLockBlocksSign(t *testing.T) {
	priv := mustPriv(t)
	sdk := newTestSDK(t, priv)

	hold := 200 * time.Millisecond
	started := make(chan struct{})
	go func() {
		sdk.LockKeyPairForTest()
		close(started)
		time.Sleep(hold)
		sdk.UnlockKeyPairForTest()
	}()
	<-started

	begin := time.Now()
	_, err := sdk.CreateEncodedSignature(hashForIndex(1))
	elapsed := time.Since(begin)
	if err != nil {
		t.Fatalf("sign: %v", err)
	}
	if elapsed < hold {
		t.Fatalf("sign returned after %v; expected to wait for write lock >= %v", elapsed, hold)
	}
}

func TestSetPrivateKeyDuringConcurrentSign(t *testing.T) {
	privA := mustPriv(t)
	privB := mustPriv(t)
	for bytes.Equal(privA, privB) {
		privB = mustPriv(t)
	}
	sdk := newTestSDK(t, privA)

	addrA, err := sdk.KeyPairAddress()
	if err != nil {
		t.Fatalf("addrA: %v", err)
	}
	tmp, err := NewSignOnlyCSDK(true, privB, "group0", "chain0")
	if err != nil {
		t.Fatalf("tmp B: %v", err)
	}
	addrB, err := tmp.KeyPairAddress()
	tmp.CloseSignOnly()
	if err != nil {
		t.Fatalf("addrB: %v", err)
	}
	if addrA == addrB {
		t.Fatalf("expected distinct addresses, both %s", addrA)
	}

	const (
		signers   = 64
		flippers  = 8
		flipsEach = 40
	)
	var (
		wg           sync.WaitGroup
		signErrs     atomic.Int64
		setErrs      atomic.Int64
		flippersDone sync.WaitGroup
		stop         atomic.Bool
	)

	flippersDone.Add(flippers)
	wg.Add(signers + flippers)
	for i := 0; i < signers; i++ {
		i := i
		go func() {
			defer wg.Done()
			for !stop.Load() {
				h := hashForIndex(i*1_000_000 + int(time.Now().UnixNano()&0xffff))
				if _, err := sdk.CreateEncodedSignature(h); err != nil {
					signErrs.Add(1)
					t.Errorf("sign under flip: %v", err)
					return
				}
				input := []byte{byte(i), byte(h[31])}
				nonce := fmt.Sprintf("flip-%d-%d", i, time.Now().UnixNano())
				if _, err := sdk.CreateSignedTransactionWithDefaultKeyPair(
					1_000_000, "2222222222222222222222222222222222222222", nonce, input, "", 0, "",
				); err != nil {
					signErrs.Add(1)
					t.Errorf("full_fields under flip: %v", err)
					return
				}
			}
		}()
	}
	for f := 0; f < flippers; f++ {
		go func() {
			defer wg.Done()
			defer flippersDone.Done()
			for i := 0; i < flipsEach; i++ {
				key := privA
				if i%2 == 1 {
					key = privB
				}
				if err := sdk.SetPrivateKey(key); err != nil {
					setErrs.Add(1)
					t.Errorf("SetPrivateKey: %v", err)
					return
				}
			}
		}()
	}

	flippersDone.Wait()
	stop.Store(true)
	wg.Wait()

	if signErrs.Load() != 0 || setErrs.Load() != 0 {
		t.Fatalf("errors during concurrent sign/SetPrivateKey: sign=%d set=%d", signErrs.Load(), setErrs.Load())
	}

	if err := sdk.SetPrivateKey(privB); err != nil {
		t.Fatalf("final SetPrivateKey B: %v", err)
	}
	addr, err := sdk.KeyPairAddress()
	if err != nil {
		t.Fatalf("final addr: %v", err)
	}
	if addr != addrB {
		t.Fatalf("final address=%s want %s", addr, addrB)
	}
	h := hashForIndex(999)
	sig, err := sdk.CreateEncodedSignature(h)
	if err != nil {
		t.Fatalf("final sign: %v", err)
	}
	if !verifyFISCOSM2Sig(t, privB, h, sig) {
		t.Fatal("final sm2.Verify failed for key B")
	}
}

func TestConcurrentReadersDoNotSerializeCompletely(t *testing.T) {
	priv := mustPriv(t)
	sdk := newTestSDK(t, priv)

	const n = 32
	h := hashForIndex(1)
	start := time.Now()
	if _, err := sdk.CreateEncodedSignature(h); err != nil {
		t.Fatalf("warmup: %v", err)
	}
	single := time.Since(start)

	start = time.Now()
	var wg sync.WaitGroup
	wg.Add(n)
	for i := 0; i < n; i++ {
		i := i
		go func() {
			defer wg.Done()
			if _, err := sdk.CreateEncodedSignature(hashForIndex(i + 2)); err != nil {
				t.Errorf("sign: %v", err)
			}
		}()
	}
	wg.Wait()
	parallel := time.Since(start)

	if parallel > single*time.Duration(n) {
		t.Fatalf("parallel signs took %v >= serial estimate %v; sign path still exclusive?", parallel, single*time.Duration(n))
	}
}
