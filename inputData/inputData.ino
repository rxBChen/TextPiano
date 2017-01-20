// // -----------------------------------
// // Controlling LEDs over the Internet
// // -----------------------------------

// int led1 = D6;
// int led2 = D7;
// int i = 0;

// void setup()
// {

//   // Here's the pin configuration, same as last time
//   pinMode(led1, OUTPUT);
//   pinMode(led2, OUTPUT);

//   // We are also going to declare a Spark.function so that we can turn the LED on and off from the cloud.
//   Spark.function("led",ledToggle);
//   // This is saying that when we ask the cloud for the function "led", it will employ the function ledToggle() from this app.

//   // For good measure, let's also make sure both LEDs are off when we start:
//   digitalWrite(led1, LOW);
//   digitalWrite(led2, LOW);
  
//   Serial.begin(9600);
//   Serial1.begin(9600);

// }

// void loop()
// {
//   if(Serial1.available())
//    Serial.write("123");
// }

// // We're going to have a super cool function now that gets called when a matching API request is sent
// // This is the ledToggle function we registered to the "led" Spark.function earlier.

// int ledToggle(String command) {
//     /* Spark.functions always take a string as an argument and return an integer.
//     Since we can pass a string, it means that we can give the program commands on how the function should be used.
//     In this case, telling the function "on" will turn the LED on and telling it "off" will turn the LED off.
//     Then, the function returns a value to us to let us know what happened.
//     In this case, it will return 1 for the LEDs turning on, 0 for the LEDs turning off,
//     and -1 if we received a totally bogus command that didn't do anything to the LEDs.
//     */
//     Serial.print("INCOMING: ");
//     Serial.println(command);
    
//     Serial1.print("PARTICLE: ");
//  Serial1.println(command);
  
//     for(i = 0; i < command.length(); i++){
//         if(command[i] != '1' && command[i] != '0'){
//             command[i] = '1';
//         }
    
//         if (command[i]=='1') {
//             digitalWrite(led1,HIGH);
//             digitalWrite(led2,HIGH);
//         }
//         else if (command[i]=='0') {
//             digitalWrite(led1,LOW);
//             digitalWrite(led2,LOW);
//         }
//         delay(500);
//     }
//     // digitalWrite(led1, LOW);
//     // digitalWrite(led2, LOW);
//     return 1;
// }


void setup() {
  Particle.function("sendData", sendData);

  Serial.begin(9600);
  Serial1.begin(9600);
}


void loop() {
  if(Serial1.available())
    Serial.write("ayy");
}


int sendData(String command) {
  Serial.print("INCOMING: ");
  Serial.println(command);

  Serial1.print("PARTICLE: ");
  Serial1.write(command);

  return 1;
}
