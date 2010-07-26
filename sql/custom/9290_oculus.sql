-- Reputazione a Oculus
DELETE FROM `creature_onkill_reputation` WHERE creature_id IN (27654, 27656, 27655, 27447, 27633, 27636, 28236, 27638, 27635, 27641, 28276, 27650, 27645, 27651, 27642, 27649, 27648, 27647, 27653, 27644, 30879, 27640, 27639, 28153);
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `IsTeamAward1`, `RewOnKillRepValue1`, `MaxStanding2`, `IsTeamAward2`, `RewOnKillRepValue2`, `TeamDependent`) VALUES 
(27654, 1037, 1052, 7, 0, 250, 7, 0, 250, 1), -- Drakos the interrogator
(27656, 1037, 1052, 7, 0, 250, 7, 0, 250, 1), -- Ley Guardian Eregos
(27655, 1037, 1052, 7, 0, 250, 7, 0, 250, 1), -- Mage lord urom
(27447, 1037, 1052, 7, 0, 250, 7, 0, 250, 1), -- Varos cloudstrider
-- Trash
(27633, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(27636, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(28236, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(27638, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(27635, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(27641, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(28276, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(27650, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(27645, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(27651, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(27642, 1037, 1052, 7, 0, 5, 7, 0, 5, 1), 
(27649, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(27648, 1037, 1052, 7, 0, 5, 7, 0, 5, 1), 
(27647, 1037, 1052, 7, 0, 5, 7, 0, 5, 1),
(27653, 1037, 1052, 7, 0, 5, 7, 0, 5, 1),
(27644, 1037, 1052, 7, 0, 5, 7, 0, 5, 1),
(30879, 1037, 1052, 7, 0, 5, 7, 0, 5, 1),
(27640, 1037, 1052, 7, 0, 5, 7, 0, 5, 1),
(27639, 1037, 1052, 7, 0, 5, 7, 0, 5, 1),
(28153, 1037, 1052, 7, 0, 5, 7, 0, 5, 1);
-- Planar Anomlay di Eregos
UPDATE creature_template SET ScriptName = 'npc_planar_anomaly' WHERE entry = 30879;
-- Arcane beam
DELETE FROM creature WHERE id = 28239;
UPDATE creature_template SET modelid3 = 0 WHERE entry = 30879;

-- Reputazione a Oculus
DELETE FROM `creature_onkill_reputation` WHERE creature_id IN (27642, 27649, 27648, 27647, 27653, 27644, 30879, 27640, 27639, 28153);
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `IsTeamAward1`, `RewOnKillRepValue1`, `MaxStanding2`, `IsTeamAward2`, `RewOnKillRepValue2`, `TeamDependent`) VALUES 
(27642, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(27649, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(27648, 1037, 1052, 7, 0, 25, 7, 0, 25, 1), 
(27647, 1037, 1052, 7, 0, 25, 7, 0, 25, 1),
(27653, 1037, 1052, 7, 0, 25, 7, 0, 25, 1),
(27644, 1037, 1052, 7, 0, 25, 7, 0, 25, 1),
(30879, 1037, 1052, 7, 0, 25, 7, 0, 25, 1),
(27640, 1037, 1052, 7, 0, 25, 7, 0, 25, 1),
(27639, 1037, 1052, 7, 0, 25, 7, 0, 25, 1),
(28153, 1037, 1052, 7, 0, 25, 7, 0, 25, 1);
