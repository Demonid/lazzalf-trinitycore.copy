DELETE FROM `spell_linked_spell` WHERE `spell_effect` in (57339,-57339);
INSERT INTO `spell_linked_spell` (`spell_trigger`,`spell_effect`,`type`,`comment`) VALUES
(21178,57339,0, 'Bear - Tank Class Passive Threat'),
(-21178,-57339,0, 'Bear - Tank Class Passive Threat'),
(7376,57339,0, 'Defensive Stance - Tank Class Passive Threat'),
(-7376,-57339,0, 'Defensive Stance - Tank Class Passive Threat'),
(48263,57339,0, 'Frost Presence - Tank Class Passive Threat'),
(-48263,-57339,0, 'Frost Presence - Tank Class Passive Threat'),
(25780,57339,0, 'Righteous Fury - Tank Class Passive Threat'),
(-25780,-57339,0, 'Righteous Fury - Tank Class Passive Threat');