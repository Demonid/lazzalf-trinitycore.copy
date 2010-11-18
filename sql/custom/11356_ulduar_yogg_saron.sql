-- Yogg-Saron emotes
DELETE FROM script_texts WHERE entry IN (-1603342, -1603343, -1603344);
INSERT INTO script_texts VALUES 
(33288, -1603342, "Portals open into Yogg-Saron's mind!", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 5, 0, 0, "YoggSaron EMOTE_PORTALS"),
(33288, -1603343, "The Illusion shatters and a path to the central chamber opens!", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 5, 0, 0, "YoggSaron EMOTE_OPEN_CHAMBER"),
(33288, -1603344, "Yogg-Saron prepares to unleash Empowering Shadows!", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 5, 0, 0, "YoggSaron EMOTE_EMPOWERING");

-- Fake npcs
UPDATE `creature_template` SET `unit_flags` = 518, `type_flags` = 0, `flags_extra` = 2, ScriptName = "npc_passive_illusions" WHERE `entry` IN (33436, 33437, 33536, 33535, 33495, 33523, 33441, 33442);