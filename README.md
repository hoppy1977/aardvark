# aardvark
An example Arduino sketch which is used to demonstrate how easy it is to build a simple Arduino circuit.<br>
Borrows heavily from https://github.com/tdicola/SMS_Pet_Food_Dish.

# AWS Cloud Components
Deploy the cloud components for this demo using the following command:<br>
`aws cloudformation deploy --template-file "./aws/aardvark.yaml" --capabilities CAPABILITY_IAM --tags "Project=aardvark" --stack-name "aardvark-demo" --parameter-overrides "Password=<password>"`