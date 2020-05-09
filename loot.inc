| Made by Shin Noir
| v 0.0.2

#Event AdvLootLeave "[MQ2] advloot #1# leave"
#Event AdvLootGive "[MQ2] advloot #1# give #2#"
#Event AdvLootEnable "[MQ2] advloot enable 1"
#Event AdvLootDisable "[MQ2] advloot disable 1"
#Event SetLoot "[MQ2] loot #1# #2#"
#Event GetBank "[MQ2] bank #1#"
#Event ParcelSent "#*#tells you, 'I will deliver#*#"
#Event Organize "[MQ2] organize"
#Event AutoSort "[MQ2] autosort"

Sub Event_AutoSort
	/echo doing auto sorting..
	/call GoToGuildHall
	/call DepositLoot
	/call GoToNeighorhood
	/call GoToGuildHall
	/echo all done!
/return


sub DepositLoot
	/if (!${Zone.ShortName.Equal[guildhallsml_int]}) {
		/echo not in guildhallsml_int
		/return
	}
:Banker
	/delay 1s
	/if (${Navigation.Active}) /goto :Banker
	/if (${NearestSpawn[npc a personal banker].Distance}>10) {
		/nav spawn npc a personal banker
		/goto :Banker
	}
	/if (${Target.ID} != ${Spawn[npc a personal banker].ID}) {
		/target npc a personal banker
		/delay 1s
	}
	/if (!${Window[BigBankWnd].Open}) {
		/click right target
		/delay 1s
	}
	/if (!${Window[BigBankWnd].Open}) {
		/echo failed to open banker window
		/endmacro
	}
	/call DoLootStuff
	/if (${Window[MerchantWnd].Open}) /nomodkey /notify MerchantWnd MW_Done_Button LeftMouseUp
    /if (${Window[guildbankwnd].Open}) /keypress esc
	/if (${Window[BigBankWnd].Open}) /keypress esc
	/if (${Window[TributeMasterWnd].Open}) /keypress esc
:Merchant
	/delay 1s
	/if (${Navigation.Active}) /goto :Banker
	/if (${NearestSpawn[npc General Supply Merchant].Distance}>10) {
		/nav spawn npc General Supply Merchant
		/goto :Merchant
	}
	/if (${Target.ID} != ${NearestSpawn[npc General Supply Merchant].ID}) {
		/target npc General Supply Merchant
		/delay 1s
	}
	/if (!${Window[MerchantWnd].Open}) {
		/click right target
		/goto :Merchant
	}
	/if (!${Window[MerchantWnd].Open}) {
		/echo failed to open MerchantWnd
		/endmacro
	}
	/call DoLootStuff
	/if (${Window[MerchantWnd].Open}) /nomodkey /notify MerchantWnd MW_Done_Button LeftMouseUp
    /if (${Window[guildbankwnd].Open}) /keypress esc
	/if (${Window[BigBankWnd].Open}) /keypress esc
	/if (${Window[TributeMasterWnd].Open}) /keypress esc
	/delay 1s
:Parcel
	/delay 1s
	/if (${Navigation.Active}) /goto :Parcel
	/if (${NearestSpawn[npc parcel].Distance}>10) {
		/nav spawn npc parcel
		/goto :Parcel
	}
	/if (${Target.ID} != ${NearestSpawn[npc parcel].ID}) {
		/echo targetting parcel
		/target npc parcel
		/goto :Parcel
	}
	/delay 1s
	/echo opening window
	/if (!${Window[MerchantWnd].Open}) {
		/click right target
		/delay 1s
	}
	/if (!${Window[MerchantWnd].Open}) {
		/echo failed to open Parcel MerchantWnd
		/endmacro
	}
	/call DoLootStuff
:GuildBank
	/delay 1s
	/if (${Navigation.Active}) /goto :Banker
	/if (${NearestSpawn[npc a_guild_banker].Distance}>10) {
		/echo going to guild banker
		/nav spawn npc a_guild_banker
		/goto :Banker
	}
	/if (${Target.ID} != ${NearestSpawn[npc a_guild_banker].ID}) {
		/echo targetting
		/target npc a_guild_banker
		/goto :GuildBank
	}
	/if (!${Window[guildbankwnd].Open}) {
		/echo right clicking
		/click right target
		/goto :GuildBank
	}
	/echo targetted guild banker
	/delay 1s
	/if (!${Window[guildbankwnd].Open}) {
		/echo failed to open a_guild_banker
		/endmacro
	}
	/delay 1s
	/echo doing loot stuff
	/call DoLootStuff
	/if (${Window[MerchantWnd].Open}) /nomodkey /notify MerchantWnd MW_Done_Button LeftMouseUp
    /if (${Window[guildbankwnd].Open}) /keypress esc
	/if (${Window[BigBankWnd].Open}) /keypress esc
	/if (${Window[TributeMasterWnd].Open}) /keypress esc
/return

Sub GoToGuildHall
:GuildHall
	/delay 1s
	/if (${Navigation.Active}) /goto :GuildHall

	/if (${Zone.ShortName.Equal[neighborhood]}) {
		/nav loc 2651 2530 117
		/if (${Math.Distance[${Me.Y},${Me.X},${Me.Z}:2651,2530,117]}<10) {
			/echo todo: figure out how to click door
			/mqp
		}
		/goto :GuildHall
	}

	/if (${Zone.ShortName.Equal[poknowledge]}) {
		/easyfind "Guild Lobby"
		/goto :GuildHall
	}
	
	/if (${Zone.ShortName.Equal[guildlobby]}) {
		/easyfind "Modest Guild Hall"
		/goto :GuildHall
	}

	/if (${Zone.ShortName.Equal[guildhallsml_int]}) {		
		/if (${Math.Distance[${Me.Y},${Me.X},${Me.Z}:0,255.-12.41]}<10) {
			/moveto loc 0 200
			/goto :GuildHall
		}
		
		/if (${Math.Distance[${Me.Y},${Me.X},${Me.Z}:0,200.-12.41]}<10) {
			/nav loc 119 130 -19.88
			/goto :GuildHall
		}
		
		/if (${Math.Distance[${Me.Y},${Me.X},${Me.Z}:119,130.-19.88]}>10 && ${Me.Heading.Degrees} != 180) {
			/nav loc 119 130 -19.88
			/goto :GuildHall
		}

		/if (${Math.Distance[${Me.Y},${Me.X},${Me.Z}:119,130.-19.88]}<10 && ${Me.Heading.Degrees} != 180) {
			/face heading 180
			/goto :GuildHall
		}
		/return
	}

	/echo unhandled zone: ${Zone.ShortName}
	/endmacro
/return


Sub GoToNeighorhood
	/declare vaultIndex int local 1
:Neighorhood
	/delay 1s
	/if (${Navigation.Active}) /goto :Neighorhood


	/if (${Zone.ShortName.Equal[guildhallsml_int]}) {		
		/if (${Math.Distance[${Me.Y},${Me.X},${Me.Z}:0,255,-12.41]}>10) {
			/nav loc 0 255 -12.41
			/goto :Neighorhood
		}
		/goto :Neighorhood
	}
	
	/if (${Zone.ShortName.Equal[neighborhood]}) {
		/if (${vaultIndex} == 1 && ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:2468,2062,34.41]}>10) {
			/echo going to vault 1
			/nav loc 2468 2062 34.41			
			/goto :Neighorhood
		}

		/if (${vaultIndex} == 1 &&  ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:2468,2062,34.41]}<=10) {
			/call DoLootStuff
			/varset vaultIndex 2
		}

		/if (${vaultIndex} == 2 &&  ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:2927,1923,34]}>10) {
			/echo going to vault 2
			/nav loc 2801 2002 34.41
			/delay 10s
			/nav loc 2927 1923 34	
			/goto :Neighorhood
		}

		/if (${vaultIndex} == 2 &&  ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:2927,1923,34]}<=10) {
			/call DoLootStuff
			/varset vaultIndex 3
		}

		/if (${vaultIndex} == 3 && ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:2911,1677,34]}>10) {
			/echo going to vault 3
			/nav loc 2911 1667 34
			/goto :Neighorhood
		}

		/if (${vaultIndex} == 3 && ${Math.Distance[${Me.Y},${Me.X},${Me.Z}:2468,2911,1677,34]}<=10) {
			/call DoLootStuff			
			/varset vaultIndex 4
		}
		/return
	}
	/echo unhandled zone: ${Zone.ShortName}
	/endmacro
/return

#Event BankGrab "[MQ2] bankgrab"
Sub Event_BankGrab
	/if (!${Window[BigBankWnd].Open}) {
		/echo failed to find bank window. Open the window and try again
		/return false
	}
	/declare i int local
	/declare j int local
	/declare slot string local
	/declare itemName string local
	/declare itemCollected int local 0
	/declare itemAction string local
	/echo grabbing vault items from bank
	/for i 1 to 24
		/varset slot bank${i}
		/if (${InvSlot[${slot}].Item.Container}) {
			/for j 1 to ${InvSlot[${slot}].Item.Container}
				/varset itemName ${InvSlot[${slot}].Item.Item[${j}].Name}
				/if (${InvSlot[${slot}].Item.Item[${j}].ID}==0) /continue
				
				/varset itemAction ${Ini[${NinjaIniFile},"${itemName.Left[1]}","${itemName}"]}
				/if (!${itemAction.Find[vault]}) /continue
				
				/if (!${Window[${slot}].Open}) /itemnotify ${slot} rightmouseup				
				/delay 5
				/if (!${Window[${slot}].Open}) {
					/echo failed to open bag ${slot}
					/return false
				}				
				
				/nomodkey /shift /itemnotify in ${slot} ${j} leftmouseup	
				/delay 5							
				/if (!${Cursor.ID}) {
					/echo failed to get item on cursor
					/return false
				}
				
				/autoinv
				/delay 5
				/if (${Cursor.ID}) {
					/echo failed to get item off cursor
					/return false
				}				
			/next j
			/if (${Window[bank${i}].Open}) /itemnotify bank${i} rightmouseup
			/delay 5
		}
	/next i
	/echo done
	|/echo failed to find ${itemName} x${itemCount}, instead found x${itemCollected}
/return

Sub Event_Organize	
	/keypress OPEN_INV_BAGS
    /declare itemIndex int local
	
    /declare j int local
    /declare lootItem string
	/declare beforePlatinum int ${Me.Platinum}
	/echo started organizing stuff
	/declare itemAction string local
	/declare lastID int local
	/declare lastPack string local
	/declare lastSlot int local

	/varset lastID -1
    | Loop through top level inventory slots for containers
    /for itemIndex 2 to 9
        | Check if top level inv slot has a bag
        /if (${InvSlot[pack${itemIndex}].Item.Container}==0) /continue

		| Set j 1 to number of bag slots and loop through slots looking for items marked sell
        /for j 1 to ${InvSlot[pack${itemIndex}].Item.Container}
			/if (${InvSlot[pack${itemIndex}].Item.Item[${j}].Type.Equal[Armor]} && ${itemIndex} != 8) {
				/nomodkey /shift /itemnotify in Pack${itemIndex} ${j} leftmouseup
				/call FreeSlotInPack 8
				/if (!${Macro.Return}) /autoinv
				/delay 5
				/continue
			}

			/if (${InvSlot[pack${itemIndex}].Item.Item[${j}].Type.Equal[Weapon]} && ${itemIndex} != 8) {
				/nomodkey /shift /itemnotify in Pack${itemIndex} ${j} leftmouseup
				/call FreeSlotInPack 8
				/if (!${Macro.Return}) /autoinv
				/delay 5
				/continue
			}

			/if (${InvSlot[pack${itemIndex}].Item.Item[${j}].Type.Equal[Augmentation]} && ${itemIndex} != 8) {
				/nomodkey /shift /itemnotify in Pack${itemIndex} ${j} leftmouseup
				/call FreeSlotInPack 8
				/if (!${Macro.Return}) /autoinv
				/delay 5
				/continue
			}

			/if (${InvSlot[pack${itemIndex}].Item.Item[${j}].Type.Equal[Quest]} && ${itemIndex} != 9) {
				/nomodkey /shift /itemnotify in Pack${itemIndex} ${j} leftmouseup
				/call FreeSlotInPack 9
				/if (!${Macro.Return}) /autoinv
				/delay 5
				/continue
			}

			/if (${InvSlot[pack${itemIndex}].Item.Item[${j}].Type.Equal[Food]} && ${itemIndex} != 4) {
				/nomodkey /shift /itemnotify in Pack${itemIndex} ${j} leftmouseup
				/call FreeSlotInPack 4
				/if (!${Macro.Return}) /autoinv
				/delay 5
				/continue
			}

			/if (${InvSlot[pack${itemIndex}].Item.Item[${j}].Type.Equal[Misc]} && ${itemIndex} != 4) {
				/nomodkey /shift /itemnotify in Pack${itemIndex} ${j} leftmouseup
				/call FreeSlotInPack 4
				/if (!${Macro.Return}) /autoinv
				/delay 5
				/continue
			}
        /next j
    /next itemIndex

	
	/keypress CLOSE_INV_BAGS
	/echo finished organizing stuff
/return


Sub FreeSlotInPack(packIndex)
    /if (${InvSlot[pack${packIndex}].Item.Container}==0) /return
	/declare k int local

	/for k 1 to ${InvSlot[pack${packIndex}].Item.Container}
		/if (${InvSlot[pack${packIndex}].Item.Item[${k}].ID} != 0) /continue
		/nomodkey /shift /itemnotify in Pack${packIndex} ${k} leftmouseup
		/return true
    /next k
/return false

Sub Organize
	/if (!${Cursor.ID}) /return false
	
	/declare destIndex int local	
    /declare k int local

	/for destIndex 2 to 9
		/for k 1 to ${InvSlot[pack${destIndex}].Item.Container}
			/if (${InvSlot[pack${destIndex}].Item.Item[${k}].ID} != 0 && ${InvSlot[pack${destIndex}].Item.Item[${k}].ID} > ${Cursor.ID}) /continue
			/nomodkey /shift /itemnotify in Pack${destIndex} ${k} leftmouseup
			/return true
		/next k
	/next destIndex
/return false

Sub Loot
	/if (!${Defined[NinjaIniFile]}) /declare NinjaIniFile string outer Loot.ini
	/if (!${Defined[IsLootHudEnabled]}) /declare IsLootHudEnabled bool outer FALSE
	/if (!${Defined[IsAutoLootingEnabled]}) /declare IsAutoLootingEnabled bool outer FALSE
	/if (${Cursor.ID}) /return
	/if (!${Defined[LastLoot]}) /declare LastLoot string outer
	/if (!${Defined[LastLootCount]}) /declare LastLootCount int outer
	/if (!${IsAutoLootingEnabled}) /return

	/if (!${Group.MasterLooter.Name.Equal[${Me.Name}]}) /return
	/if (${AdvLoot.LootInProgress}) /return
	/if (!${Defined[itemName]}) /declare itemName string local
	/declare itemIndex int local
	
	/declare dropCount int local
	/declare npcName string local

	/declare itemAction string local
	/for itemIndex 1 to ${AdvLoot.SCount}
		/if (${Cursor.ID}) /return
		/if (${AdvLoot.LootInProgress}) /delay 1s
		/varset itemName ${AdvLoot.SList[${itemIndex}]}
		/if (${itemName.Equal[NULL]}) /continue
	
		/varset npcName ${AdvLoot.SList[${itemIndex}].Corpse.Name.Mid[1, ${AdvLoot.SList[${itemIndex}].Corpse.Name.Find['s_corpse]}-1]}
		/varset npcName ${npcName.Mid[1,${Math.Calc[${npcName.Length}-1]}]}
		/varset dropCount ${Ini[loot\${Zone.ShortName}.ini,${npcName},${AdvLoot.SList[${itemIndex}].ID}-DropCount]}
		/varcalc dropCount ${dropCount}+1

		/varset itemAction ${Ini[${NinjaIniFile},"${itemName.Left[1]}","${itemName}"]}

		/if (${itemAction.Equal[sell]}) /varset itemAction keep
		/if (${itemAction.Equal[guildbank]}) /varset itemAction keep
		/if (${itemAction.Find[bank|]}) /varset itemAction keep
		/if (${itemAction.Equal[personaltribute]}) /varset itemAction keep
		/if (${itemAction.Equal[guildtribute]}) /varset itemAction keep
		/if (${itemAction.Find[vault]}) /varset itemAction keep
		/if (${itemAction.Find[parcel|]}) /varset itemAction keep
		/if (${itemAction.Equal[bazaar]}) /varset itemAction keep
		/if (${itemAction.Equal[ignore]}) /varset itemAction leave
		|/if (${itemAction.Equal[keep]}) /varset itemAction keep

		/if (${AdvLoot.SList[${itemIndex}].Never}) /varset itemAction leave
		/if (${AdvLoot.SList[${itemIndex}].Greed} || ${AdvLoot.SList[${itemIndex}].Need}) /varset itemAction keep

		/if (${itemAction.Equal[leave]}) {		
			/ini "loot\${Zone.ShortName}.ini" "${npcName}" "${AdvLoot.SList[${itemIndex}].ID}-DropCount" "${dropCount}"
			
			/advloot shared "${itemName}" leave
			/echo Leaving ${itemName}
		}

		/if (${itemAction.Equal[keep]}) {
			/ini "loot\${Zone.ShortName}.ini" "${npcName}" "${AdvLoot.SList[${itemIndex}].ID}-DropCount" "${dropCount}"
			/if (${LastLoot.Equal[${itemName}]}) {
				/varcalc LastLootCount ${LastLootCount}+1
				/advloot shared "${itemName}" giveto ${Me.Name} ${LastLootCount}
				/echo trying to loot ${LastLootCount}x of ${itemName}
				/return
			} else {
				/varset LastLootCount 1
			}
			/advloot shared "${itemName}" giveto ${Me.Name} 1
			/varset LastLoot ${itemName}
			/echo Looting ${itemName}
			|/delay 5s
			/return
		}
		
		
	/next itemIndex
/return


sub WindowBuffer
	/if (!${Defined[winIndex]}) /declare winIndex int local
	/echo making buffer
	/for winIndex 1 to 7
		/keypress esc
		/delay 1
	/next winIndex
	/for winIndex 1 to 7
		/echo ${i}
		/nomodkey /altkey /itemnotify ${winIndex} leftmouseup
		|/echo ${InvSlot[${i}].Item.Name} vs ${Window[ItemDisplayWindow].Child[IDW_ItemInfo1].Text}
		/if (${InvSlot[${winIndex}].Item.Name.Equal[${Window[ItemDisplayWindow].Child[IDW_ItemInfo1].Text}]}) {
			|escape last window once
			/keypress esc
			/return
		}
		/delay 5
	/next winIndex
	/echo itemdisplaywindow buffer failed
	/endmacro
/return

sub IsNeededToBeCollected
	/declare mainWnd string local ItemDisplayWindow
	/declare child string local IDW_CollectedLabel
	/if (!${Defined[LastItem]}) /declare LastItem string outer
	/if (!${Window[${mainWnd}].Text.Length}) {
		/echo didn't get window text?
		/endmacro
	}


	/varset LastItem ${Window[${mainWnd}].Text}
	/if (${Window[${mainWnd}].Child[${child}].Text.Equal[Collected]}) {
		/echo already collected ${Window[${mainWnd}].Text}
		/return false
	}

	
	/if (!${Window[${mainWnd}].Child[${child}].Text.Length}) {
		/echo can't collect ${Window[${mainWnd}].Text}
		/return false
	}
/return true

#Event Collect "[MQ2] collect"
Sub Event_Collect
	/call WindowBuffer
	/declare attemptCounter int local
	/declare slot int local
	/declare pack string local
	/echo scanning for collectables
	/if (!${Defined[itemName]}) /declare itemName string local

	/if (!${Zone.ShortName.Equal[neighborhood]}) {
		/echo not in neighorhood
		|/return
	}
	
	/if (!${Window[RealEstateItemsWnd].Open}) {
		/windowstate RealEstateItemsWnd open
		/echo opening vault
		/delay 5
	}
	/keypress OPEN_INV_BAGS
	/declare vaultIndex int local
	/declare collectCount int local
	/for vaultIndex 1 to ${Window[RealEstateItemsWnd].Child[REIW_ItemList].Items}
		/nomodkey /notify RealEstateItemsWnd REIW_ItemList listselect ${vaultIndex}
		/delay 5
		/nomodkey /notify RealEstateItemsWnd REIW_ItemList rightmouse ${vaultIndex}
		/delay 5
		
		/varset itemName ${Window[RealEstateItemsWnd].Child[REIW_ItemList].List[${vaultIndex},2]}
		/echo ${itemName}
		/nomodkey /notify Inspect menuselect

		/delay 2s
		/call IsNeededToBeCollected
		/keypress esc
		/if (!${Macro.Return}) /continue
		/varcalc vaultIndex ${vaultIndex}-1
		/echo collecting it
		/nomodkey /ctrlkey /notify RealEstateItemsWnd REIW_Move_Inventory_Button leftmouseup
		/delay 1s
		/varcalc collectCount ${collectCount}+1
		/if (!${InvSlot[pack${Float[${Math.Calc[${Select[${FindItem[=${itemName}].ItemSlot},22,23,24,25,26,27,28,29,30,31,32]}-1]}].Int}].Item.Container}) {
			/nomodkey /shift /itemnotify ${FindItem[=${itemName}].InvSlot} rightmouseup
		} else {
			/nomodkey /shift /itemnotify in Pack${Math.Calc[${FindItem[=${itemName}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${itemName}].ItemSlot2}+1]} rightmouseup
		}
	/next vaultIndex
	/keypress CLOSE_INV_BAGS
	/echo finished collecting ${collectCount} items

/return true

/return

Sub Event_GetBank(line, itemName) 
	/declare itemCount int local -1

	/if (${itemName.Find[ ]} && ${Int[${itemName.Right[-${itemName.Find[ ]}]}]}) {		
		/varset itemCount ${Int[${itemName.Right[-${itemName.Find[ ]}]}]}
		/varset itemName ${itemName.Left[${Math.Calc[${itemName.Find[ ]}-1]}]}
	}
	/if (${itemCount}==0) /varset itemCount 1

	/call TakeFromBank "${itemName}" "${itemCount}"
	/if (!${Macro.Return}) {
		/echo failed to get item from bank
		/return
	}
	
	/if (${Window[BigBankWnd].Open}) /keypress esc
/return

Sub Event_SetLoot(line, itemAction, itemOption)
	/if (!${Defined[NinjaIniFile]}) /declare NinjaIniFile string outer Loot.ini
	/if (!${Defined[IsLootHudEnabled]}) /declare IsLootHudEnabled bool outer FALSE
	/if (${itemAction.Lower.Equal[parcel]}) /varset itemAction "${itemAction}|${itemOption}"
	/if (${itemAction.Lower.Equal[vault1]}) /varset itemAction "${itemAction}|${itemOption}"
	/if (${itemAction.Lower.Equal[vault2]}) /varset itemAction "${itemAction}|${itemOption}"
	/if (${itemAction.Lower.Equal[vault3]}) /varset itemAction "${itemAction}|${itemOption}"
	/if (${itemAction.Lower.Equal[bank]}) /varset itemAction "${itemAction}|${itemOption}"
	/if (${itemAction.Lower.Equal[enable]}) {
		/varcalc IsAutoLootingEnabled TRUE	
		/echo auto looting is now TRUE
		/return
	}
	/if (${itemAction.Lower.Equal[disable]}) {
		/varcalc IsAutoLootingEnabled FALSE
		/echo auto looting is now FALSE
		/return
	}
	/if (${itemAction.Lower.Equal[begin]}) {
		/call DoLootStuff
		/return
	}
	/if (${itemAction.Lower.Equal[info]}) {
		/if (${Cursor.ID}) {
			/call LootInfo "${Cursor.ID}" "${Cursor.Name}"
			/return
		}
		
	}

	/if (${itemAction.Lower.Equal[mode]}) {
		/loadhud loot
		/return
	}
	/call EditIniLoot "${itemAction.Lower}"
/return

Sub EditIniLoot(itemAction)
	/if (!${Defined[NinjaIniFile]}) /declare NinjaIniFile string outer Loot.ini
	/if (!${Defined[IsLootHudEnabled]}) /declare IsLootHudEnabled bool outer FALSE
	/if (!${Defined[IsAutoLootingEnabled]}) /declare IsAutoLootingEnabled bool outer FALSE
	/if (${Cursor.ID}==0) {
		/echo No item on cursor to ${itemAction}.
		/return
	}
	/if (!${itemAction.Equal[bazaar]} && !${itemAction.Equal[sell]} && !${itemAction.Find[vault1]} && !${itemAction.Find[vault3]} && !${itemAction.Find[parcel]} && !${itemAction.Find[vault2]} && !${itemAction.Find[bank]} && !${itemAction.Equal[personaltribute]} && !${itemAction.Equal[guildtribute]} && !${itemAction.Equal[ignore]}) {
		/echo Unsupported action: ${itemAction}
		/return
	}

	/echo -- Setting ${Cursor.Name} to ${itemAction}
	/ini "${NinjaIniFile}" ${Cursor.Name.Left[1]} "${Cursor.Name}" ${itemAction}
/return

Sub LootInfo(int itemID, string itemName)
	/call LootInfoRecipe ${itemID} "${itemName}"
	/call LootInfoMerchant ${itemID} "${itemName}"
	/echo info done
/return

Sub LootInfoRecipe(int itemID, string itemName)
	/sqlite tradeskiller/tradeskiller.db recipes SELECT skill, trivial, name FROM item_recipes WHERE is_required = 1 AND (item1_id = ${itemID} OR item2_id = ${itemID} OR item3_id = ${itemID} OR item4_id = ${itemID} OR item5_id = ${itemID} OR item6_id = ${itemID} OR item7_id = ${itemID} OR item8_id = ${itemID}) LIMIT 1
	/if (${sqlite.resultcode[recipes]} != 0) {
		/echo failed to query recipes ${sqlite.status[recipes]}
		/return false
	}
	
	/if (!${sqlite.rows[recipes]}) {
		/sqlite tradeskiller/tradeskiller.db recipes SELECT skill, trivial, name FROM item_recipes WHERE (item1_id = ${itemID} OR item2_id = ${itemID} OR item3_id = ${itemID} OR item4_id = ${itemID} OR item5_id = ${itemID} OR item6_id = ${itemID} OR item7_id = ${itemID} OR item8_id = ${itemID}) LIMIT 1
		/if (${sqlite.resultcode[recipes]} != 0) {
			/echo failed to query recipes ${sqlite.status[recipes]}
			/return false
		}
		
		/if (!${sqlite.rows[recipes]}) {
			/return false
		}
		/echo ${itemName} is used in ${Skill[${sqlite.result[recipes 1 skill]}]} recipe ${sqlite.result[recipes 1 name]} trivial ${sqlite.result[recipes 1 trivial]}
		/return false
	}
	/echo ${itemName} is used in required ${Skill[${sqlite.result[recipes 1 skill]}]} recipe ${sqlite.result[recipes 1 name]} trivial ${sqlite.result[recipes 1 trivial]}

	
/return


Sub LootInfoMerchant(int itemID, string itemName)
	/sqlite tradeskiller/tradeskiller.db merchants SELECT merchants.name FROM item_merchants INNER JOIN merchants ON merchant_id = id WHERE item_id = ${itemID} LIMIT 1
	/if (${sqlite.resultcode[merchants]} != 0) {
		/echo failed to query merchants ${sqlite.status[merchants]}
		/return false
	}
	
	/if (!${sqlite.rows[merchants]}) {
		/return false
	}
	/echo ${itemName} is sold by ${sqlite.result[merchants 1 name]}
/return

Sub Event_AdvLootEnable(line, index) 
	/if (!${Defined[IsAutoLootingEnabled]}) /declare IsAutoLootingEnabled bool outer FALSE
	/varset IsAutoLootingEnabled TRUE
	/echo auto looting items is now enabled
/return

Sub Event_AdvLootDisable(line, index) 
	/if (!${Defined[IsAutoLootingEnabled]}) /declare IsAutoLootingEnabled bool outer FALSE
	/varset IsAutoLootingEnabled FALSE
	/echo auto looting items is now disabled
/return


Sub Event_AdvLootLeave(line, index) 
	/if (!${Defined[itemName]}) /declare itemName string local
	/declare dropCount int local
	/declare npcName string local
	/if (!${Defined[i]}) /declare i int local 
	/varset i ${index}
	/varset itemName ${AdvLoot.SList[${itemIndex}]}
	/varset npcName ${AdvLoot.SList[${itemIndex}].Corpse.Name.Mid[1, ${AdvLoot.SList[${itemIndex}].Corpse.Name.Find['s_corpse]}-1]}
	/varset npcName ${npcName.Mid[1,${Math.Calc[${npcName.Length}-1]}]}
	/varset dropCount ${Ini[loot\${Zone.ShortName}.ini,${npcName},${AdvLoot.SList[${itemIndex}].ID}-DropCount]}
	/varcalc dropCount ${dropCount}+1
	/ini "loot\${Zone.ShortName}.ini" "${npcName}" "${AdvLoot.SList[${itemIndex}].ID}-DropCount" "${dropCount}"
	/advloot shared "${itemName}" leave			
	/delay 2s
	/if (${AdvLoot.LootInProgress}) /delay 2s
	/bc Left ${itemName} dropped by ${npcName} ${dropCount} times on corpse
/return


Sub EventAdv_LootGive(line, index, targetName) 
	/declare i int local
	/if (!${Defined[itemName]}) /declare itemName string local
	/declare dropCount int local
	/declare npcName string local
	/varset i ${index}
	/varset itemName ${AdvLoot.SList[${itemIndex}]}
	/varset npcName ${AdvLoot.SList[${itemIndex}].Corpse.Name.Mid[1, ${AdvLoot.SList[${itemIndex}].Corpse.Name.Find['s_corpse]}-1]}
	/varset npcName ${npcName.Mid[1,${Math.Calc[${npcName.Length}-1]}]}
	/varset dropCount ${Ini[loot\${Zone.ShortName}.ini,${npcName},${AdvLoot.SList[${itemIndex}].ID}-DropCount]}
	/varcalc dropCount ${dropCount}+1
	/ini "loot\${Zone.ShortName}.ini" "${npcName}" "${AdvLoot.SList[${itemIndex}].ID}-DropCount" "${dropCount}"
	/advloot shared "${itemName}" giveto ${targetName} 1
	/delay 2s
	/if (${AdvLoot.LootInProgress}) /delay 2s
	/bc Gave ${itemName} dropped by ${npcName} ${dropCount} times to ${targetName}
/return


Sub DoLootStuff
	/keypress OPEN_INV_BAGS
	/declare someString string local

	/if (${someString.Equal[Words]}) /echo ok, good
	/varset someString Words
	/if (${someString.Equal[NULL]}) /echo ok, good

    /declare itemIndex int local
    /declare j int local
    /declare k int local
    /declare lootItem string
	/declare beforePlatinum int ${Me.Platinum}
	/echo starting loot stuff
	/declare itemAction string local

	/if (${Window[MerchantWnd]} && ${Window[MerchantWnd].Child[MW_Retrieve_All_Button].Enabled}) {
		/echo grabbing stuff from parcel
		/nomodkey /shiftkey /notify MerchantWnd MW_Retrieve_All_Button leftmouseup
		/delay 10s ${Window[MerchantWnd].Child[MW_ParcelCountNumber].Text.Equal[Total Parcel Count: 0]}
	}


   | Top level inv has its own loop since ${FindItem[=${itemName}].InvSlot} looks at top lev inv slots first in Sub SellToVendor for the item to sell.
   | Loop through top level inventory slots for sellable items
   /for k 1 to 10
        /if (${InvSlot[pack${k}].Item.Container}>0) /continue
        /if (${InvSlot[pack${k}].Item.ID}==0) /continue

		/varset lootItem ${InvSlot[pack${k}].Item}
		/varset itemAction ${Ini[${NinjaIniFile},"${lootItem.Left[1]}","${lootItem}"]}

		/if (${InvSlot[pack${k}].Item.Collectible}) /varset itemAction vault3|foo
		/if (!${Bool[${itemAction}]}) /continue
		/if (${itemAction.Equal[sell]}) /call SellToVendor "${lootItem}"
		/if (${itemAction.Equal[guildbank]}) /call DepositToGuildBank "${lootItem}"
		/if (${itemAction.Find[bank|]}) /call DepositToBank "${lootItem}" "${itemAction}"
		/if (${itemAction.Equal[personaltribute]}) /call DonateToPersonalTribute "${lootItem}"
		/if (${itemAction.Equal[guildtribute]}) /call DonateToGuildTribute "${lootItem}"
		/if (${itemAction.Find[vault1|]}) /call DepositToVault 1 "${lootItem}" "${itemAction.Replace["vault1|",""]}"
		/if (${itemAction.Find[vault2|]}) /call DepositToVault 2 "${lootItem}" "${itemAction.Replace["vault2|",""]}"
		/if (${itemAction.Find[vault3|]}) /call DepositToVault 3 "${lootItem}" "${itemAction.Replace["vault3|",""]}"
		/if (${itemAction.Find[parcel|]}) /call SendToParcel "${lootItem}" "${itemAction}"
		/if (${itemAction.Equal[bazaar]}) /call SellOnBazaar "${lootItem}"
    /next k

    | Loop through top level inventory slots for containers
    /for itemIndex 1 to 10
        | Check if top level inv slot has a bag
        /if (${InvSlot[pack${itemIndex}].Item.Container}==0) /continue
        
		| Set j 1 to number of bag slots and loop through slots looking for items marked sell
        /for j 1 to ${InvSlot[pack${itemIndex}].Item.Container}
            /if (${InvSlot[pack${itemIndex}].Item.Item[${j}].ID}==0) /continue

			
            /varset lootItem ${InvSlot[pack${itemIndex}].Item.Item[${j}].Name}
			/varset itemAction ${Ini[${NinjaIniFile},"${lootItem.Left[1]}","${lootItem}"]}

			/if (${InvSlot[pack${itemIndex}].Item.Item[${j}].Collectible}) /varset itemAction vault3|foo
			/if (!${Bool[${itemAction}]}) /continue
			/if (${itemAction.Equal[sell]}) /call SellToVendor "${lootItem}"
			/if (${itemAction.Equal[guildbank]}) /call DepositToGuildBank "${lootItem}"
			/if (${itemAction.Find[bank|]}) /call DepositToBank "${lootItem}" ${itemAction}
			/if (${itemAction.Equal[personaltribute]}) /call DonateToPersonalTribute "${lootItem}"
			/if (${itemAction.Equal[guildtribute]}) /call DonateToGuildTribute "${lootItem}"
			/if (${itemAction.Find[vault1|]}) /call DepositToVault 1 "${lootItem}" "${itemAction.Replace["vault1|",""]}"
			/if (${itemAction.Find[vault2|]}) /call DepositToVault 2 "${lootItem}" "${itemAction.Replace["vault2|",""]}"
			/if (${itemAction.Find[vault3|]}) /call DepositToVault 3 "${lootItem}" "${itemAction.Replace["vault3|",""]}"
			/if (${itemAction.Find[parcel|]}) /call SendToParcel "${lootItem}" "${itemAction}"
			/if (${itemAction.Equal[bazaar]}) /call SellOnBazaar "${lootItem}"
        /next j
    /next itemIndex

    | Prevent spam from Sell event after selling items in Sub SellStuff
    /doevents flush Sell

	
	/keypress CLOSE_INV_BAGS
    /if (${Window[MerchantWnd].Open}) /nomodkey /notify MerchantWnd MW_Done_Button LeftMouseUp
    /if (${Window[guildbankwnd].Open}) /keypress esc
	/if (${Window[BigBankWnd].Open}) /keypress esc
	/if (${Window[MerchantWnd].Open}) /nomodkey /notify MerchantWnd MW_Done_Button LeftMouseUp
	/if (${Window[TributeMasterWnd].Open}) /keypress esc
    
	/if (${Math.Calc[${Me.Platinum}-${beforePlatinum}]}>0) /echo ${Math.Calc[${Me.Platinum}-${beforePlatinum}]} total platinum earned
	
	/if (${Cursor.ID}) /autoinventory
	/echo finished loot stuff
	
/return

Sub TakeFromBank(string itemName, int itemCount)
	/if (!${Window[BigBankWnd].Open}) {
		/echo failed to find bank window. Open the window and try again
		/return false
	}
	/declare i int local
	/declare j int local
	/declare slot string local
	/declare targetName string local
	/declare itemCollected int local 0

	/if (${itemCount}==0) /varset itemCount 1
	/if (${itemCount}<-1) {
		/echo invalid itemCount ${itemCount}
		/return false
	}
	
	/if (${itemCount}==-1) {
		|/echo looking for all ${itemName} in bank
	}
	/if (${itemCount}>0) {
		|/echo looking for ${itemName} x${itemCount} in bank
	}
	
	/for i 1 to 24
		/varset slot bank${itemIndex}
		/if (${InvSlot[${slot}].Item.Container}) {
			/for j 1 to ${InvSlot[${slot}].Item.Container}
			  	/if (${InvSlot[${slot}].Item.Item[${j}].ID}==0) /continue			
				  
				/varset targetName ${InvSlot[${slot}].Item.Item[${j}].Name}
				/if (!${targetName.Upper.Equal[${itemName.Upper}]}) /continue
			
				/if (!${Window[${slot}].Open}) /itemnotify ${slot} rightmouseup
				
				/delay 5 ${Window[${slot}].Open}
				/if (!${Window[${slot}].Open}) {
					/echo failed to open bag ${slot}
					/return false
				}

				/echo found ${targetName} at ${slot} ${j}
				
				/varcalc itemCollected ${itemCollected}+${InvSlot[${slot}].Item.Item[${j}].Stack}

				/delay 5 ${Cursor.ID}
				/nomodkey /shift /itemnotify in ${slot} ${j} leftmouseup				
				/delay 5
				/if (!${Cursor.ID}) {
					/echo failed to get item on cursor
					/return false
				}
				
				/autoinv
				/delay 5
				/if (${Cursor.ID}) {
					/echo failed to get item off cursor
					/return false
				}

				/if (${itemCollected}>=${itemCount}) {
					/echo collected ${itemName} x ${itemCollected}
					/return true
				}
			/next j
			/if (${Window[bank${itemIndex}].Open}) /itemnotify bank${itemIndex} rightmouseup			
		}

        /if (${InvSlot[${slot}].Item.Container}>0) /continue
        /if (${InvSlot[${slot}].Item.ID}==0) /continue
		/varset targetName ${InvSlot[${slot}].Item}
	/next i

	|/echo failed to find ${itemName} x${itemCount}, instead found x${itemCollected}
/return false


Sub DepositToBank(itemName, targetBank)
	/if (!${Bool[${targetBank}]}) /return
	/varset targetBank ${targetBank.Replace["bank|",""]}
	
	/if (!${Me.Name.Equal[${targetBank}]}) {
		/call SendToParcel "${itemName}" "${targetBank}"
		/return
    }
    /if (!${Window[BigBankWnd].Open}) {
		/return
	}
:deposit

	/if (${FindItem[=${itemName}].InvSlot}<=22) /return
	/echo Depositing ${itemName}
	/if (!${InvSlot[pack${Float[${Math.Calc[${Select[${FindItem[=${itemName}].ItemSlot},22,23,24,25,26,27,28,29,30,31,32]}-1]}].Int}].Item.Container}) {
		/nomodkey /shift /itemnotify ${FindItem[=${itemName}].InvSlot} leftmouseup
	} else {
		/nomodkey /shift /itemnotify in Pack${Math.Calc[${FindItem[=${itemName}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${itemName}].ItemSlot2}+1]} leftmouseup
	}
	/delay 1
	/nomodkey /notify BigBankWnd BIGB_AutoButton leftmouseup
	/delay 1
	/if (${FindItem[=${itemName}].InvSlot}) /goto :deposit
/return

Sub DepositToGuildBank(itemName)
    /if (!${Window[GuildBankWnd].Open}) /return
:deposit
	/if (${Window[GuildBankWnd].Child[GBANK_DepositCountLabel].Text.Arg[2,:]}==0) {
		/echo Guildbank Deposited Items is full.
		/return
	}
	/if (${FindItem[=${itemName}].InvSlot}<=22) /return
	/echo Depositing ${itemName}
	/if (!${InvSlot[pack${Float[${Math.Calc[${Select[${FindItem[=${itemName}].ItemSlot},22,23,24,25,26,27,28,29,30,31,32]}-1]}].Int}].Item.Container}) {
		/nomodkey /shift /itemnotify ${FindItem[=${itemName}].InvSlot} leftmouseup
	} else {
		/nomodkey /shift /itemnotify in Pack${Math.Calc[${FindItem[=${itemName}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${itemName}].ItemSlot2}+1]} leftmouseup
	}
	/delay 5
	/nomodkey /notify GuildBankWnd GBANK_DepositButton leftmouseup
	/delay 2s
	/if (${FindItem[=${itemName}].InvSlot}) /goto :deposit
/return

Sub SellToVendor(itemName)
	/if (!${Window[MerchantWnd].Open} || ${Window[MerchantWnd].Child[MW_MerchantName].Text.Find[Parcel]}) /return
:sell
	/echo Selling ${itemName}
	/if (${InvSlot[pack${Select[${FindItem[=${itemName}].ItemSlot},23,24,25,26,27,28,29,30,31,32]}].Item.Container}) {
		/nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${itemName}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${itemName}].ItemSlot2}+1]} leftmouseup
	} else {
		/nomodkey /itemnotify ${FindItem[=${itemName}].InvSlot} leftmouseup
	}
	/delay 5
	/nomodkey /shiftkey /notify merchantwnd MW_Sell_Button leftmouseup
	/doevents
	/if (${Macro.Return.Equal[TRUE]}) /return
	/delay 2s
	/if (${FindItemCount[=${itemName}]}) /goto :sell
/return

Sub DonateToPersonalTribute(itemName)
	/echo tribute?
    /if (!${Window[TributeMasterWnd].Open} || !${Window[TributeMasterWnd].Text.Equal[Tribute Master]}) /return
:sell
	/echo donating ${itemName} to personal tribute
	/if (${InvSlot[pack${Select[${FindItem[=${itemName}].ItemSlot},23,24,25,26,27,28,29,30,31,32]}].Item.Container}) {
		/nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${itemName}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${itemName}].ItemSlot2}+1]} leftmouseup
	} else {
		/nomodkey /itemnotify ${FindItem[=${itemName}].InvSlot} leftmouseup
	}
	/delay 5
	/nomodkey /shiftkey /notify TributeMasterWnd TMW_DonateButton leftmouseup
	/doevents
	/if (${Macro.Return.Equal[TRUE]}) /return
	/delay 2s
	/if (${FindItemCount[=${itemName}]}) /goto :sell
/return


Sub DonateToGuildTribute(itemName)
	/if (!${Window[TributeMasterWnd].Open} || !${Window[TributeMasterWnd].Text.Equal[Guild Tribute Master]}) /return
    /if (!${Window[MerchantWnd].Open}) /return
:sell
	/echo Selling ${itemName}
	/if (${InvSlot[pack${Select[${FindItem[=${itemName}].ItemSlot},23,24,25,26,27,28,29,30,31,32]}].Item.Container}) {
		/nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${itemName}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${itemName}].ItemSlot2}+1]} leftmouseup
	} else {
		/nomodkey /itemnotify ${FindItem[=${itemName}].InvSlot} leftmouseup
	}
	/delay 5
	/nomodkey /shiftkey /notify TributeMasterWnd TMW_DonateButton leftmouseup
	/doevents
	/if (${Macro.Return.Equal[TRUE]}) /return
	/delay 2s
	/if (${FindItemCount[=${itemName}]}) /goto :sell
/return


Sub DepositToVault(targetVault, itemName, playerName)
	/declare attemptCounter int local
	/declare isWindowOpen bool local
	/declare slot int local
	/declare pack string local

	/if (!${Defined[itemName]}) /declare itemName string local

	/if (!${Me.Name.Equal[${playerName}]}) {
		/call SendToParcel "${itemName}" "${playerName.Replace["vault|",""]}"
		/return
    }

	/declare isItemInVault bool local FALSE
	/varset isWindowOpen ${Window[RealEstateItemsWnd].Open}
	/if (!${Zone.ShortName.Equal[neighborhood]}) /return

	/if (!${Window[RealEstateItemsWnd].Open}) {
		/windowstate RealEstateItemsWnd open
		/echo opening vault
		/delay 5
	}
	/if (!${Window[RealEstateItemsWnd].Open}) {
		/return
	}
	/if (${targetVault} == 1 && ${Window[RealEstateItemsWnd].Text.NotEqual[Items - Storm Crossed Village, 105 Guild Way]}) {
		/return
	}
	/if (${targetVault} == 2 && ${Window[RealEstateItemsWnd].Text.NotEqual[Items - Storm Crossed Village, 102 Guild Way]}) {
		/return
	}
	/if (${targetVault} == 3 && ${Window[RealEstateItemsWnd].Text.NotEqual[Items - Storm Crossed Village, 103 Guild Way]}) {
		/return
	}
	
	|/for vaultIndex 1 to ${Window[RealEstateItemsWnd].Child[REIW_ItemList].Items}
	|	/varset itemName ${Window[RealEstateItemsWnd].Child[REIW_ItemList].List[${vaultIndex},2]}
	|	/if (${itemName.Length} > 0) /varset isItemInVault TRUE	
	|/next vaultIndex
	|/if (!${isItemInVault}) /return

:nextItem
	/varset attemptCounter 0
	/varset pack ${InvSlot[pack${Select[${FindItem[=${itemName}].ItemSlot},23,24,25,26,27,28,29,30,31,32]}].Item.InvSlot.Pack}
	/varset slot ${FindItem[=${itemName}].InvSlot}
	/if (${FindItemCount[=${itemName}]}==0) /return TRUE	
	
:vault
	/varcalc attemptCounter ${attemptCounter}+1
	/if (${attemptCounter}>10) {
		/echo attempted too many times, exiting with failure
		/return FALSE
	}
	/if (${Cursor.ID}) {
		/echo ${Cursor.Name} on cursor, /autoinv	
		/autoinv
		/delay 1s
	}
	
	/if (${InvSlot[pack${Select[${FindItem[=${itemName}].ItemSlot},23,24,25,26,27,28,29,30,31,32]}].Item.Container}) {
		/nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${itemName}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${itemName}].ItemSlot2}+1]} leftmouseup
	} else {
		/nomodkey /itemnotify ${FindItem[=${itemName}].InvSlot} leftmouseup
	}

	/delay 5
	/nomodkey /shiftkey /notify RealEstateItemsWnd REIW_Move_Closet_Button leftmouseup
	/doevents
	/if (${Macro.Return.Equal[TRUE]}) /return
	/delay 1s
	/if (${FindItemCount[=${itemName}]}>0) /goto :nextItem
/return TRUE

Sub SendToParcel(itemName, targetPlayer)
	/varset targetPlayer ${targetPlayer.Replace["parcel|",""]}
	/if (${targetPlayer.Equal[NULL]}) /return
	/if (${targetPlayer.Equal[${Me.Name}]}) /return
    /if (!${Window[MerchantWnd].Open} || !${Window[MerchantWnd].Child[MW_MerchantName].Text.Find[Parcel]}) /return

	/notify MerchantWnd MW_MerchantSubWindows tabselect 3
	/delay 1s	
	
	/echo sending parcel ${itemName} to ${targetPlayer}	
:send
	/if (!${Window[MerchantWnd].Open}) /return
	/if (!${Window[MerchantWnd].Child[MW_Send_Button].Enabled} && !${FindItemCount[=${itemName}]}) /return
	/if (${InvSlot[pack${Select[${FindItem[=${itemName}].ItemSlot},23,24,25,26,27,28,29,30,31,32]}].Item.Container}) {
		/nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${itemName}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${itemName}].ItemSlot2}+1]} leftmouseup
	} else {
		/nomodkey /itemnotify ${FindItem[=${itemName}].InvSlot} leftmouseup
	}
	/nomodkey /shiftkey /notify MerchantWnd MW_Send_To_Edit leftmouseup
	/if (!${Window[MerchantWnd].Child[MW_Send_Button].Enabled} && ${FindItemCount[=${itemName}]}) {
		/delay 1s
		/goto :send
	}
	/call ClearChat
	/call TypeIn "${targetPlayer}"
	/delay 5
	/nomodkey /shiftkey /notify MerchantWnd MW_Send_Button leftmouseup
	/if (${FindItemCount[=${itemName}]}) /goto :send
/return

Sub TypeIn(message)
	/declare char string local 
	/declare charIndex int local 
	/for charIndex 1 to ${message.Length} 
		/varset char ${message.Mid[${charIndex},1]} 
		/if (!${char.Length}) { 
			/nomodkey /keypress Space chat 
		} else { 
			/if (${char.Left[1].Equal[(]}) /return
			/if (${char.Left[1].Equal[)]}) /return
			/nomodkey /keypress ${char} chat 
		} 
	/next charIndex
/return


Sub SellOnBazaar(itemName, targetPlayer)
	/if (!${Bool[${targetBank}]}) /return
	/varset targetBank ${targetBank.Replace["bank|",""]}

	/if (${Ini[${NinjaIniFile},Settings,BazaarParcel].Length}) {
		/if (!${Me.Name.Equal[${Ini[${NinjaIniFile},Settings,BazaarParcel]]}) {
		   /call SendToParcel "${itemName}" "${Ini[${NinjaIniFile},Settings,BazaarParcel]}"
		   /return
	   }
    }

	/if (${itemName.Count[|]}<1) /echo ${itemName} has invalid destination
    /if (!${Window[MerchantWnd].Open}) /return
	/notify MerchantWnd MW_MerchantSubWindows tabselect 3
	/delay 1s
	
:sell
	/echo Sending Parcel ${itemName} to ${targetPlayer}
	/if (${InvSlot[pack${Select[${FindItem[=${itemName}].ItemSlot},23,24,25,26,27,28,29,30,31,32]}].Item.Container}) {
		/nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${itemName}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${itemName}].ItemSlot2}+1]} leftmouseup
	} else {
		/nomodkey /itemnotify ${FindItem[=${itemName}].InvSlot} leftmouseup
	}
	/nomodkey /shiftkey /notify MerchantWnd MW_Send_To_Edit leftmouseup
	/call ClearChat
	/call TypeIn "${targetPlayer}"
	/delay 5
	/return
	/nomodkey /shiftkey /notify MerchantWnd MW_Send_Button leftmouseup
	/if (${FindItemCount[=${itemName}]}) /goto :sell
/return

Sub ClearChat
    :Try1
		/ctrlkey /shiftkey /keypress home chat
		/delay 5
		/ctrlkey /shiftkey /keypress delete chat
		/delay 5
		/if (${Window[MerchantWnd].Child[MW_Send_To_Edit].Text.Length}>0) /goto :Try1
/return