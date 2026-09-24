:: =================== ROOT ====================
:: Generate ROT key pair and cert request
:: Private Key PEM pass phrase, Please Enter: 12345678
openssl ecparam -out ecdsa256\rot-key.pem -name secp256r1 -genkey
openssl req -key ecdsa256\rot-key.pem -new -out ecdsa256\rot-req.csr

:: Self signed cert. The rot-cert.pem will be used to sign MBL cert.
openssl x509 -req -in ecdsa256\rot-req.csr -signkey ecdsa256\rot-key.pem -out ecdsa256\rot-cert.pem -days 3650

:: Convert a key pem file to txt. You can copy the public key hash to ROTPK_HASH[].
openssl pkey -in ecdsa256\rot-key.pem -pubout -out ecdsa256\rot-key.txt -text

:: Delete intermediate file
if exist ecdsa256\rot-req.csr del ecdsa256\rot-req.csr

:: You can check the cert info use the following command.
openssl x509 -in ecdsa256\rot-cert.pem -out ecdsa256\rot-cert.txt -text

:end