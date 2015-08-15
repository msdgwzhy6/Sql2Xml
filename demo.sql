
CREATE DATABASE /*!32312 IF NOT EXISTS*/ `ph_center_db` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `ph_center_db`;
--
-- Table structure for table `friend_category`
--
CREATE TABLE `friend_category` (
  `cate_id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '好友分组表',
  PRIMARY KEY (`cate_id`,`owner_type`),
  KEY `owner_id` (`owner_id`,`owner_type`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `ph_logs1` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `ph_logs1`;
--
-- Table structure for table `game_logs`
--

DROP TABLE IF EXISTS `game_logs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `game_logs` (
  `log_id` bigint(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '日志ID',
  `group_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '服务器组ID',
  `server_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '服务器ID',
  `status` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '日志状态 1 有效日志  2 删除日志',
  `type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '日志类型',
  `sub_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '日志子类型',
  `info` text NOT NULL COMMENT '备注信息',
  `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '日志创建时间',
  PRIMARY KEY (`log_id`,`log_ids`),
  KEY `create_time` (`create_time`) USING BTREE,
  KEY `platform_id` (`platform_id`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=332148 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

CREATE TABLE `friend_category` (
  `cate_id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '好友分组表',
  PRIMARY KEY (`cate_id`,`owner_type`),
  KEY `owner_id` (`owner_id`,`owner_type`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
