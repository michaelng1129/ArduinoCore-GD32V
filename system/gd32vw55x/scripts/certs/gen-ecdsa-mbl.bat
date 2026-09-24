:: =================== MBL and PROT ====================
:: Delete MBL key and cert if exist
del ecdsa256\mbl-key.pem
del ecdsa256\mbl-cert.pem
 
:: Generate MBL key pair and cert request
:: Private Key PEM pass phrase, Please Enter: 12345678
openssl ecparam -out ecdsa256\mbl-key.pem -name secp256r1 -genkey
openssl req -key ecdsa256\mbl-key.pem -new -out ecdsa256\mbl-req.csr

:: Signed by rot-cert.pem
openssl x509 -req -in ecdsa256\mbl-req.csr -out ecdsa256\mbl-cert.pem -signkey ecdsa256\mbl-key.pem -CA ecdsa256\rot-cert.pem -CAkey ecdsa256\rot-key.pem -CAcreateserial -days 3650

:: Convert a key pem file to txt. Just for debug help.
openssl pkey -in ecdsa256\mbl-key.pem -pubout -out ecdsa256\mbl-key.txt -text

:: Convert a cert pem file to txt. Just for debug help.
openssl x509 -in ecdsa256\mbl-cert.pem -out ecdsa256\mbl-cert.txt -text

:: Delete intermediate file
if exist ecdsa256\mbl-req.csr del ecdsa256\mbl-req.csr

