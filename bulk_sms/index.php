<?php 


   
    // include_once('db.php');
    // Be sure to include the file you've just downloaded
    require_once('AfricasTalkingGateway.php');
    // Specify your authentication credentials
    $username   = "GSM_message";
    $apikey     = "745122a78f15e838d2d1044f555e2ea21dcdfd39be9dadd400f5b26629e9ef5d";
    // Specify the numbers that you want to send to in a comma-separated list
    // Please ensure you include the country code (+254 for Kenya in this case)


    
    
    //$customers_data = array();
    if(isset($_POST['GPS'])){
        
        
        // $cURL = curl_init();
        // $setopt_array = array(CURLOPT_URL => "https://webhook.site/0434c2f1-5da1-4a51-9cea-2a73f3b07bd1",    CURLOPT_RETURNTRANSFER => true, CURLOPT_HTTPHEADER => array()); 
        // curl_setopt($ch, CURLOPT_POSTFIELDS, "THIS HELLO");
        // curl_setopt_array($cURL, $setopt_array);
        // $json_response_data = curl_exec($cURL);
        // print_r($json_response_data);
        // curl_close($cURL);
    
    $name = "Hello";
    $phone = "+254757674142";
    $message = "Hi this is Esther, i thing I'm in danger " . $_POST['GPS'] . ",\r\n";

    echo $_POST['GPS'];
    echo "<br>";
    echo $_POST['temp'];
    echo "<br>";
    echo $_POST['BPM'];
    echo "<br>";



    // Create a new instance of our awesome gateway class
    $gateway    = new AfricasTalkingGateway($username, $apikey);
    /*************************************************************************************
    NOTE: If connecting to the sandbox:
    1. Use "sandbox" as the username
    2. Use the apiKey generated from your sandbox application
    https://account.africastalking.com/apps/sandbox/settings/key
    3. Add the "sandbox" flag to the constructor
    $gateway  = new AfricasTalkingGateway($username, $apiKey, "sandbox");
    **************************************************************************************/
    // Any gateway error will be captured by our custom Exception class below, 
    // so wrap the call in a try-catch block
    try 
    { 
        // Thats it, hit send and we'll take care of the rest. 
         //$results = $gateway->sendMessage($phone, $message);
                
        // foreach($results as $result2) {
        // // status is either "Success" or "error message"
        // echo " Number: " .$result2->number;
        // echo " Status: " .$result2->status;
        // echo " StatusCode: " .$result2->statusCode;
        // echo " MessageId: " .$result2->messageId;
        // echo " Cost: "   .$result2->cost."<br>";
        // }
    } catch ( AfricasTalkingGatewayException $ex )
    {
        echo "Encountered an error while sending: ".$ex->getMessage();
    }


    }



?>
