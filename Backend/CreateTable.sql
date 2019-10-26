/* Mysql.. */

CREATE TABLE `measurement` (
  `MesurementId` bigint(20) NOT NULL AUTO_INCREMENT,
  `DeviceId` int(11) DEFAULT NULL,
  `Time` datetime DEFAULT NULL,
  `DeviceName` varchar(10) DEFAULT NULL,
  `Measurement` varchar(10) DEFAULT NULL,
  `Value` float(8,2) DEFAULT NULL,
  `Unit` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`MesurementId`)
)