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
+ UseFrameBlockEvent
+ EntityExplodeEvent
+ BlockExplodeEvent
+ RespawnAnchorExplodeEvent  
+ BlockExplodedEvent
+ CommandBlockExecuteEvent
+ ProjectileHitBlockEvent
+ MobDieEvent
+ ActorHurtEvent
+ ProjectileHitEntityEvent
+ WitherDestroyEvent
+ PistonPushEvent
+ FarmDecayEvent
+ PlayerDropItemEvent
+ PlayerConsumeTotemEvent
+ PlayerChangeSlotEvent
+ PlayerSetArmorEvent
+ PlayerUseRespawnAnchorEvent
+ PlayerPullFishingHookEvent
+ PlayerSleepEvent
+ ExecutingCommandEvent
+ PlayerChangeDimension

在控制台使用 `levilogger rotate` 来保存正在写入的日志。

日志位置： `plugins/LeviLogger/data/log/` 。

## Config

+ `version` = 2，配置文件版本
+ `locateName` i18n语言，支持 `zh_CN` 与 `en_US`
+ `maxLine` 单个日志文件最大行数（正整数）
+ `xxxEvent`
  + `log` 是否输出（布尔）
  + `print` 是否打印到控制台（布尔）
  + `noOutputContent` 不进行输出的内容（字符串列表）。若一行log包含此列表中的任意一项的内容，则不进行输出。
  + `printFormat` 打印到控制台的格式（字符串）。程序会对其中的一些特殊内容做替换处理，具体见下表：

| 特殊内容 | 替换内容 |
| --- | --- |
| `{{self}}` | 事件主体 |
| `{{event}}` | 事件名称（已翻译） |
| `{{selfUUID}}` | 主体UUID（可能为空） |
| `{{dim}}` | 主体维度名称（已翻译）（可能为空） |
| `{{x}}` | 主体X坐标（可能为空） |
| `{{y}}` | 主体Y坐标（可能为空） |
| `{{z}}` | 主体Z坐标（可能为空） |
| `{{target}}` | 事件目标（可能为空） |
| `{{tx}}` | 目标X坐标（可能为空） |
| `{{ty}}` | 目标Y坐标（可能为空） |
| `{{tz}}` | 目标Z坐标（可能为空） |
| `{{info}}` | 附加信息（可能为空） |

### 默认配置

#### 默认不进行输出的事件

+ PlayerJumpEvent
+ PlayerSneakingEvent
+ PlayerSneakedEvent
+ PlayerSprintingEvent
+ PlayerSprintedEvent
+ PlayerSwingEvent
+ ProjectileHitBlockEvent
+ MobDieEvent
+ ActorHurtEvent
+ ProjectileHitEntityEvent
+ WitherDestroyEvent
+ PistonPushEvent

#### 默认进行打印的事件

+ PlayerChatEvent - `<{{self}}> ({{x}}, {{y}}, {{z}}) {{info}}`
+ PlayerJoinEvent - `<{{self}}> ({{x}}, {{y}}, {{z}}) {{event}}`
+ CommandBlockExecuteEvent - `<{{self}}> ({{x}}, {{y}}, {{z}}) {{info}}`
+ ExecutingCommandEvent - `<{{self}}> ({{x}}, {{y}}, {{z}}) {{info}}`
+ PlayerChangeDimension - `<{{self}}> ({{x}}, {{y}}, {{z}}) {{event}} {{info}}`

## License

GPLv3 License
