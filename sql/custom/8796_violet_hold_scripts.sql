update creature_template set ScriptName='mob_dragons_vh' where entry IN (30660,30695,30661,30662,30663,30664,30666,30668,30667,32191);
update creature_template set ScriptName='npc_azure_saboteur_vh' where entry = 31079;
DELETE FROM `script_texts` WHERE `entry` = -1703065;
INSERT INTO `script_texts` (`npc_entry`, `entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `sound`, `type`, `language`, `emote`, `comment`) VALUES (30658, -1703065, 'You fail the contenment!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 3, 0, 0, 'Violet_Hold_Failed');