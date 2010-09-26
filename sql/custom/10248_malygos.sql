UPDATE creature_template SET  flags_extra =  flags_extra | 0x2 WHERE entry = 31253; -- Alexstrazsa
UPDATE creature_model_info SET combat_reach = 15 WHERE modelid = 26752;
DELETE FROM script_texts WHERE entry = -1616035;
INSERT INTO script_texts (npc_entry, entry, content_default, TYPE, COMMENT) VALUE
(28859, -1616035, "A Power Spark forms from a nearby rift!", 5, "Malygos WHISPER_POWER_SPARK");
UPDATE script_texts SET content_default = "My patience has reached its limit. I will be rid of you!" WHERE entry = -1616005;
UPDATE instance_template SET script = 'instance_eye_of_eternity' WHERE map = 616;
UPDATE `gameobject_template` SET `ScriptName` = 'go_malygos_iris' WHERE `entry` IN (193960,193958); 
UPDATE creature_template SET ScriptName = 'boss_malygos' WHERE entry = 28859;
UPDATE creature_template SET ScriptName = 'mob_nexus_lord' WHERE entry = 30245;
UPDATE creature_template SET ScriptName = 'mob_scion_of_eternity' WHERE entry = 30249;
UPDATE creature_template SET ScriptName = 'mob_power_spark' WHERE entry = 30084;
UPDATE creature_template SET ScriptName = 'npc_arcane_overload' WHERE entry = 30282;