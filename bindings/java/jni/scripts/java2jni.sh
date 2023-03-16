#!/bin/bash
dirpath="$(cd "$(dirname "$0")" && pwd)"
cd $dirpath/../

# TODO: check if javah command available

JNI_INCLUDE_PATH="src/main/c/jni"
JAVA_SOURCE_PATH="src/main/java/"

BASIC_PKG_PREFIX="org.fisco.bcos.sdk.jni"

function convert_java_to_jni() {
    local dir=$1
    echo " ===>>> dir = $dir"
    for file in $(ls "$dir")
    do
        if [ -f $1"/"$file ];then
            echo $1"/"$file
        elif [ -d $1"/"$file ];then
            convert_java_to_jni $1"/"$file
        fi
    done
}

# echo "=== Convert Java To JNI === "
# [ ! -d ${JAVA_SOURCE_PATH} ] && {
#     echo "${JAVA_SOURCE_PATH} not exist."
#     exit 0
# }
# convert_java_to_jni ${JAVA_SOURCE_PATH}

# classes="rpc.RpcJniObj amop.AmopJniObj event.EventSubJniObj BcosSDKJniObj"
classes="utilities.keypair.KeyPairJniObj utilities.tx.TransactionBuilderJniObj utilities.receipt.ReceiptBuilderJniObj utilities.signature.SignatureJniObj"

for class in ${classes}
do
    javah -d ${JNI_INCLUDE_PATH} -cp ${JAVA_SOURCE_PATH} "${BASIC_PKG_PREFIX}.${class}"
    echo " ===> class: ${BASIC_PKG_PREFIX}.${class}"
done

echo "=== Convert Java To JNI Ending === "

