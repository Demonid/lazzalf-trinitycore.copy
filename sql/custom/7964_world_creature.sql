UPDATE `creature` SET `position_z`=`position_z`+'0.5' WHERE `id`='28366';

SET @GUID := 1001500; -- Highest GUID in the table
SET @ID := 23033; -- Invisible Stalker (Floating)

INSERT INTO `creature` (`guid`,`id`,`map`,`position_x`,`position_y`,`position_z`,`orientation`,`spawndist`) VALUES
(@GUID+1,@ID,'571','5314.51','2703.69','409.430115','-0.890117','0'),
(@GUID+2,@ID,'571','5316.25','2977.04','409.419250','-0.820303','0');