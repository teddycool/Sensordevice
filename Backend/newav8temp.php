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
SELECT AVG(Value) AS Value, Measurement, Time, HOUR(Time) AS Hour
FROM measurement 
WHERE DeviceName IN  ('av8_k1', 'av8_s1') AND  Measurement IN ( 'temp', 'otemp', 'ftemp')  AND Time BETWEEN DATE_SUB( NOW() , INTERVAL 1 DAY ) AND NOW()
GROUP BY Hour , Measurement, Time
Order BY Time Asc, Measurement
EOD;


#Value	Measurement	Time	Hour


$jsondata = '{ "cols": [{"label": "Tid", "type": "string"},';
$jsondata = $jsondata . '{"label": "Inne i stugan", "type": "number"},';
$jsondata = $jsondata . '{"label": "Under huset", "type": "number"},';
$jsondata = $jsondata . '{"label": "Ute vid luckan", "type": "number"}';
$jsondata = $jsondata . '],';
$jsondata = $jsondata . '"rows": [';

#Hour, M1, M2, M3...


if ($result1 = $conn->query($sql1)) {
    $prevhourvalue = Null;
    $otemp=Null;
    $ftemp=Null;
    $itemp=Null;
    while ($row = $result1->fetch_assoc()) {
        if($row["Measurement"]=='otemp'){
            $otemp=$row["Value"];
        }
        if($row["Measurement"]=='ftemp'){
            $ftemp=$row["Value"];
        }
        if($row["Measurement"]=='temp'){
            $itemp=$row["Value"];
        }
        
        if ($prevhourvalue != $row["Hour"]){               
            if($otemp!= Null and $ftemp!=Null and  $itemp!=Null and $prevhourvalue!= Null){
                #Write to json..
                $jsondata = $jsondata  . '{"c": [{"v":"' . $prevhourvalue . '"},'. '{"v":"' . $itemp . '"},'. '{"v":"' . $ftemp . '"},'. '{"v":"' . $otemp . '"}' . ']},'; 
            }
            
            #    $otemp=Null;
            #    $ftemp=Null;
            #   $itemp=Null;
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
