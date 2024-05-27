# LeviLogger

LeviLamina BDS Player Behavior Logger

## Install

Use lip:

`lip install github.com/odorajbotoj/LeviLogger`

## Usage

记录服务器事件。目前提供的事件有：

+ PlayerAddExperienceEvent
+ PlayerAttackEvent
+ PlayerChangePermEvent
+ PlayerChatEvent
+ PlayerConnectEvent
+ PlayerDestroyBlockEvent
+ PlayerDieEvent
+ PlayerInteractBlockEvent
+ PlayerJoinEvent
+ PlayerJumpEvent
+ PlayerLeaveEvent
+ PlayerPickUpItemEvent
+ PlayerPlacingBlockEvent
+ PlayerPlacedBlockEvent
+ PlayerRespawnEvent
+ PlayerSneakingEvent
+ PlayerSneakedEvent
+ PlayerSprintingEvent
+ PlayerSprintedEvent
+ PlayerSwingEvent
+ PlayerUseItemEvent
+ PlayerUseItemOnEvent
+ useFrameBlockEvent
+ entityExplodeEvent
+ blockExplodeEvent
+ respawnAnchorExplodeEvent  
+ blockExplodedEvent
+ commandBlockExecuteEvent
+ projectileHitBlockEvent
+ mobDieEvent
+ actorHurtEvent
+ projectileHitEntityEvent
+ witherDestroyEvent
+ pistonPushEvent
+ farmDecayEvent
+ playerDropItemEvent
+ playerConsumeTotemEvent
+ playerChangeSlotEvent
+ playerSetArmorEvent
+ playerUseRespawnAnchorEvent
+ playerPullFishingHookEvent
+ playerSleepEvent

在控制台使用 `levilogger rotate` 来保存正在写入的日志。

日志位置： `plugins/LeviLogger/data/log/` 。

## Config

+ `version` = 1，配置文件版本
+ `locateName` i18n语言，支持 `zh_CN` 与 `en_US`
+ `maxLine` 单个日志文件最大行数
+ `xxxEvent`
  + `log` 是否输出
  + `noOutputContent` 不进行输出的内容

## License

GPLv3 License
