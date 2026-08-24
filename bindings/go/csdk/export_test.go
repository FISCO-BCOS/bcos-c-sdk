package csdk

// LockKeyPairForTest acquires the exclusive keyPair write lock (simulates SetPrivateKey hold).
func (csdk *CSDK) LockKeyPairForTest() { csdk.keyPairMutex.Lock() }

// UnlockKeyPairForTest releases the exclusive keyPair write lock.
func (csdk *CSDK) UnlockKeyPairForTest() { csdk.keyPairMutex.Unlock() }
