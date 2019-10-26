<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


require_once('config.php');

// Create connection
$conn = new mysqli(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
//SELECT * FROM measurement WHERE `Time` BETWEEN DATE_SUB( CURDATE( ) ,INTERVAL 1 DAY ) AND CURDATE( )
//SELECT Time, Value FROM measurement where DeviceName = 'av8_s1' and Measurement IN ( 'otemp') AND  `Time` BETWEEN DATE_SUB( CURDATE( ) ,INTERVAL 1 DAY ) AND CURDATE( ) ORDER BY Time Desc
//$sql1 = "SELECT Time, Value FROM measurement where DeviceName = 'av8_k1' and Measurement IN ( 'temp')  AND  `Time` BETWEEN DATE_SUB( NOW() ,INTERVAL 1 DAY ) AND NOW() ORDER BY Time Desc" ;

$sql1 = <<<EOD
SELECT AVG(Value) AS Value, DeviceName, Time, HOUR(Time) AS Hour
FROM measurement 
WHERE DeviceName IN  ('av8_k1', 'av8_s1') AND  Measurement IN ( 'bat')  AND Time BETWEEN DATE_SUB( NOW() , INTERVAL 1 DAY ) AND NOW()
GROUP BY Hour , DeviceName, Time
Order BY Time Asc, DeviceName
EOD;


#Value	Measurement	Time	Hour


$jsondata = '{ "cols": [{"label": "Tid", "type": "string"},';
$jsondata = $jsondata . '{"label": "Batteristatus kökssensor", "type": "number"},';
$jsondata = $jsondata . '{"label": "Batteristatus utesensor", "type": "number"}';
$jsondata = $jsondata . '],';
$jsondata = $jsondata . '"rows": [';

#Hour, M1, M2, M3...


if ($result1 = $conn->query($sql1)) {
    $prevhourvalue = Null;
    $kbat=Null;
    $ubat=Null;
    while ($row = $result1->fetch_assoc()) {
        if($row["DeviceName"]=='av8_s1'){
            $ubat=$row["Value"];
        }
        if($row["DeviceName"]=='av8_k1'){
            $kbat=$row["Value"];
        }
        
        if ($prevhourvalue != $row["Hour"]){               
            if($kbat!= Null and $ubat!=Null and $prevhourvalue!= Null){
                #Write to json..
                $jsondata = $jsondata  . '{"c": [{"v":"' . $prevhourvalue . '"},'. '{"v":"' . $kbat . '"},'. '{"v":"' . $ubat . '"}'.  ']},'; 
            }
                $prevhourvalue = $row["Hour"];
            
        }
        
}
}
    $result1->free();    

$conn->close();
#$len = strlen($jsondata)-1;
$newjsondata = substr($jsondata,0,-1);

$jsondata = $newjsondata . "]}";
echo $jsondata;

?>
