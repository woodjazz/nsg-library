@rem example (DEBUG): make CONFIG=Debug serve
@rem example (RELEASE): make serve

@set PATH=e:/python27;%PATH%
@set NACL_SDK_ROOT=e:/nacl_sdk/pepper_canary
@%NACL_SDK_ROOT%/tools/make.exe %*
