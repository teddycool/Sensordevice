<!DOCTYPE html>
<html><body>
<?php

require_once('config.php');

// Create connection
$conn = new mysqli(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT Time, DeviceName, measurement, Value, Unit FROM measurement ORDER BY Time Desc limit 100" ;

echo '<table cellspacing="5" cellpadding="5">
      <tr> 
        <td>Time</td> 
        <td>Device</td> 
        <td>Measurement</td> 
        <td>Value</td> 
        <td>Unit</td> 
      </tr>';
 
if ($result = $conn->query($sql)) {
    while ($row = $result->fetch_assoc()) {
        $row_time = $row["Time"];
        $row_DeviceName = $row["DeviceName"];
        $row_measurement = $row["measurement"];
        $row_value = $row["Value"];
        $row_unit = $row["Unit"]; 
      
        echo '<tr> 
                <td>' . $row_time . '</td> 
                <td>' . $row_DeviceName . '</td> 
                <td>' . $row_measurement . '</td> 
                <td>' . $row_value . '</td> 
                <td>' . $row_unit . '</td>
              </tr>';
    }
    $result->free();
}

$conn->close();
?> 
</table>
</body>
</html>

