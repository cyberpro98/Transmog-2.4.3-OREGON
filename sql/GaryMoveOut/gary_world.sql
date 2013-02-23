-- Add Name Admin, GameMaster and Guard Announces.
REPLACE INTO `mangos_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES
(12002, '|cffffff00[|c1f4DF620Guard|r |c100FFFF0%s|c1f4DF620 Announces|cffffff00]:|r %s|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(12001, '|cffffff00[|c1f4DF620GameMaster|r |c100FFFF0%s|c1f4DF620 Announces|cffffff00]:|r %s|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(12000, '|cffffff00[|c1f4DF620Administrator|r |c100FFFF0%s|c1f4DF620 Announces|cffffff00]:|r %s|r', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `command` (`name`, `security`, `help`) VALUES
('msgadm', 3, 'Syntax: .adm $announcement.\nSend an announcement to all online players, displaying the name of the sender.'),
('msggm', 2, 'Syntax: .adm $announcement.\nSend an announcement to all online players, displaying the name of the sender.'),
('msgguard', 1, 'Syntax: .msg $announcement.\nSend an announcement to all online players, displaying the name of the sender.');
