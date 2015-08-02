| Ninjadvloot.inc v1.6
|Modified to only loot new Items that are equal or above the loot Value you set. -nytemyst
|Took some of ModBot'S Looting code to add setting Max number of drops to loot plus quest support.-nytemyst
|Added the Ability to Have toons loot only what'S in their personal Ini section and ignore everything else.-nytemyst
#Event ALCantLoot	"#*#may not loot this corpse#*#"
#event ALCantLoot	"#*#This Corpse is using advanced looting#*#"
#Event SomeoneElse "#*#Someone is already looting that corpse#*#"
#event CurrentInventoryProfit "[MQ2] Show me the money!" 
#include general.inc
Sub SetupAdvLootVars
/declare ALDontLoot      int    outer
/declare CorpseRotTimer  timer  outer
/call ALLoadVar ${Me} LootMobs      TRUE  bool
/call ALLoadVar ${Me} LootValue  1000    int
/call ALLoadVar ${Me} CorpseRadius  100   int
/call ALLoadVar ${Me} MobsTooClose  50    int
/call ALLoadVar ${Me} CorpseRotTime 440s  string
/call ALLoadVar ${Me} ReportLoot    TRUE bool
/call ALLoadVar ${Me} LootChannel   1  string
/call ALLoadVar ${Me} LootTrash     FALSE bool
/call ALLoadVar ${Me} DoQuest TRUE bool
/call ALLoadVar ${Me} QuestOnly FALSE bool
/call ALLoadVar ${Me} AllTradeSkillItems FALSE bool
/if (!${Defined[${AlertList}]}) /declare AlertList int outer 200
/squelch /alert clear 25
/squelch /alias /profit /echo Show me the money!
/return

Sub ALLoadVar(IniSection,IniVar,IniValue,VarType)
/if (!${Defined[${IniVar}]} && ${Defined[VarType]}) /declare ${IniVar} ${VarType} outer
/declare IniString string local ${Ini[Loot.ini,${IniSection},${IniVar},NOTFOUND]}
/varset ${IniVar} ${IniString}
/if (${IniString.Equal["NOTFOUND"]}) {
  /if (${IniString.Equal["NOTFOUND"]}) /varset ${IniVar} ${IniValue}
  /ini "Loot.ini" "${IniSection}" "${IniVar}" "${${IniVar}}"
}
/return

Sub LootCorpse
/declare i          int    local
/declare LootList   string local
/declare x          int local
/declare CurrentItem string local
/declare CurrIni string local
/declare CurrPersonalIni string local
/declare CurrQuestIni string local
/declare CurrGeneralIni string local
/call CheckCursor
/loot
/delay 1s ${Corpse.Open}
/doevents ALCantLoot
/doevents SomeoneElse
/if (${Macro.Return.Find[SomeoneLooting]}) /return
/if (${Target.ID}==${ALDontLoot} && ${Spawn[${ALDontLoot}].ID}) /squelch /alert add 25 id ${ALDontLoot}
/delay 1s ${Corpse.Items} || ${Target.ID}==${ALDontLoot}
/if (${Corpse.Open} && ${Corpse.Items}) {
	/if (${CorpseID}) /squelch /alert add 25 id ${CorpseID}
	/declare loottotal  int    local
	:ALlootlag
	/doevents Dead
	/varset loottotal ${Corpse.Items}
	/delay 1s ${loottotal}!=${Corpse.Items}
	/if (${loottotal}!=${Corpse.Items}) /goto :ALlootlag
	/for i 1 to ${loottotal}
		/multiline ; /varset CurrentItem ${If[${Corpse.Item[${i}].Name.Find[,]},${Corpse.Item[${i}].Name.Arg[1,,]}${Corpse.Item[${i}].Name.Arg[2,,]},${Corpse.Item[${i}].Name}]}; /varset CurrIni ${Ini["loot.ini","${Corpse.Item[${i}].Name.Left[1]}","${CurrentItem}"]}; /varset CurrPersonalIni ${Ini["loot.ini","${Me}","${CurrentItem}"]}; /varset CurrQuestIni ${Ini["loot.ini","Quests","${CurrentItem}"]}; /varset CurrGeneralIni ${Ini["loot.ini","General Loot","${CurrentItem}"]}	
		/if (${CurrIni.Left[5].Equal[Keep|]} && ${FindItemCount[${Corpse.Item[${i}]}]}>=${CurrIni.Arg[2,|]}) /next i
		/if (${CurrPersonalIni.Left[5].Equal[Keep|]} && ${FindItemCount[${Corpse.Item[${i}]}]}>=${CurrPersonalIni.Arg[2,|]}) {
			/echo ${CurrPersonalIni.Arg[2,|]} is the max i can loot for ${Corpse.Item[${i}]}
			/if (${Corpse.Item[${i}].ID})	/varset LootList ${LootList}${Corpse.Item[${i}]}
			/goto :nextitem
		}
		/if (${CurrGeneralIni.Left[5].Equal[Keep|]} && ${FindItemCount[${Corpse.Item[${i}]}]}>=${CurrGeneralIni.Arg[2,|]}) {
			/echo ${CurrGeneralIni.Arg[2,|]} is the max i can loot for ${Corpse.Item[${i}]}
			/if (${Corpse.Item[${i}].ID})	/varset LootList ${LootList}${Corpse.Item[${i}]}
			/goto :nextitem
		}
		/if (${CurrIni.Equal[Destroy]}) {
			/if (!${QuestOnly}) /call LootItem ${i} Destroy left
			/goto :nextitem
		}
 	/if (!${LootTrash}) /goto :personal
 	/if (${Corpse.Item[${i}].ID}) /echo ${Corpse.Item[${i}].Name} is worth ${Math.Calc[${Corpse.Item[${i}].Value}/1000]}pp
	/if (${Corpse.Item[${i}].ID} && !${Select[${CurrIni},Ignore,Keep,Destroy,Sell,Buy,${Me.Name}]} && !${CurrIni.Find[Quest|]} && !${CurrIni.Left[5].Equal[Keep|]} && !${CurrQuestIni.Find[Quest|]} ) {
		/if (${Corpse.Item[${i}].NoDrop}) /ini "loot.ini" "${Corpse.Item[${i}].Name.Left[1]}" "${Corpse.Item[${i}]}" Ignore
		/if (${Corpse.Item[${i}].Value}<${LootValue} && !${Corpse.Item[${i}].NoDrop} && !${Select[${CurrIni},Ignore,Keep,Destroy,Quest,Sell]} && ${CurrIni.Left[5].Equal[NULL]}) /ini "loot.ini" "${Corpse.Item[${i}].Name.Left[1]}" "${Corpse.Item[${i}]}" Ignore
		/if (!${Corpse.Item[${i}].NoDrop} && !${Select[${CurrIni},Ignore,Keep,Destroy,Quest,Sell]} && ${CurrIni.Left[5].Equal[NULL]}) /ini "loot.ini" "${Corpse.Item[${i}].Name.Left[1]}" "${Corpse.Item[${i}]}" Keep
		/varset CurrIni ${Ini["Loot.ini","${Corpse.Item[${i}].Name.Left[1]}","${CurrentItem}"]}
	}
	/if (${CurrIni.NotEqual[Destroy]} && !${Me.FreeInventory} && (!${FindItemCount[=${Corpse.Item[${i}].Name}]} || (${FindItemCount[=${Corpse.Item[${i}].Name}]} && ${Corpse.Item[${i}].Stackable} && !${Corpse.Item[${i}].FreeStack})) || (${Corpse.Item[${i}].Lore} && ${FindItem[${Corpse.Item[${i}]}].ID}) || ${CurrIni.Equal[Ignore]}) /varset LootList ${LootList}${Corpse.Item[${i}]},
	/if ((${Select[${CurrIni},Keep,Sell]} && (!${Corpse.Item[${i}].Lore} || !${FindItem[${Corpse.Item[${i}]}].ID}) && ${Me.FreeInventory}) || (${CurrIni.Left[5].Equal[Keep|]}) || (${Me.FreeInventory} && ${Corpse.Item[${i}].Collectible}) || (${AllTradeSkillItems} && ${Me.FreeInventory} && ${Corpse.Item[${i}].Tradeskills})) {
		/call LootItem ${i} Keep Right
	} 
	/if (${CurrIni.Equal[Destroy]}) /call LootItem ${i} Destroy Left
	:personal
	/if (${CurrPersonalIni.Equal[Keep]} && (!${Corpse.Item[${i}].Lore} || !${FindItem[${Corpse.Item[${i}]}].ID}) && (${Me.FreeInventory} || (${FindItemCount[=${Corpse.Item[${i}].Name}]} && ${Corpse.Item[${i}].Stackable} && ${Corpse.Item[${i}].FreeStack}))) {
		/call LootItem ${i} Keep Right
	}
	/if (${CurrPersonalIni.Left[5].Equal[Keep|]} && ${Me.FreeInventory} || ${CurrGeneralIni.Left[5].Equal[Keep|]}) {
		/if (!${Corpse.Item[${i}].Lore}) /call LootItem ${i} Keep Right
		/if (${Corpse.Item[${i}].Lore} && !${FindItem[${Corpse.Item[${i}]}].ID}) /call LootItem ${i} Keep Right
		/if (${Corpse.Item[${i}].ID})	/varset LootList ${LootList}${Corpse.Item[${i}]},
	} 
	/if (${FindItemCount[=${Corpse.Item[${i}].Name}]}>=${CurrQuestIni.Arg[2,|]} && ${CurrQuestIni.Arg[2,|].NotEqual[NULL]}) {
		/varset LootList ${LootList}${Corpse.Item[${i}]},
		next i
	}
    /if (${DoQuest} && ${CurrIni.Left[6].Equal[Quest|]} && ${FindItemCount[=${Corpse.Item[${i}].Name}]}<${CurrIni.Arg[2,|]} && (${Me.FreeInventory} || (${FindItemCount[=${Corpse.Item[${i}].Name}]} && ${Corpse.Item[${i}].Stackable} && ${Corpse.Item[${i}].FreeStack})) || ${DoQuest} && ${CurrQuestIni.Left[6].Equal[Quest|]} && ${FindItemCount[=${Corpse.Item[${i}].Name}]}<${CurrQuestIni.Arg[2,|]} && (${Me.FreeInventory} || ${FindItemCount[=${Corpse.Item[${i}].Name}]} && ${Corpse.Item[${i}].Stackable} && ${Corpse.Item[${i}].FreeStack}) ) {
		/if (${CurrIni.Arg[3,|].Length} || ${CurrQuestIni.Arg[3,|].Length}) {
    		/if (!${Window[TaskWnd].Open} && !${Window[TaskWnd].Child[TASK_TaskList].Items}) /keypress alt+q
    		/delay 1s ${Window[TaskWnd].Open}
    		/for x 1 to ${Window[TaskWnd].Child[TASK_TaskList].Items}
    			/if (${Window[TaskWnd].Child[TASK_TaskList].List[${x},2].Equal[${CurrIni.Arg[3,|]}]} || ${Window[TaskWnd].Child[TASK_TaskList].List[${x},2].Equal[${CurrQuestIni.Arg[3,|]}]}  ) /call LootItem ${i} Keep Right
    		/next x
    		/if (${Window[TaskWnd].Open}) /keypress alt+q
    	} else {
			/if (!${Window[TaskWnd].Child[TASK_TaskList].List[${x},2].Equal[${CurrIni.Arg[3,|]}]} || !${Window[TaskWnd].Child[TASK_TaskList].List[${x},2].Equal[${CurrQuestIni.Arg[3,|]}]}) /echo Please specify quest name for ${Corpse.Item[${i}].Name}.  Format for "loot.ini" is =Quest|<count>|<quest name>|  
    	}
    }  
	:NextItem
	/next i
	/if (${Corpse.Items} && ${ReportLoot} && ${LootList.Length}) {
		/nomodkey /keypress /
		/nomodkey /keypress ${LootChannel} chat
		/nomodkey /notify LootWnd LW_BroadcastButton LeftMouseUp
		/nomodkey /keypress enter chat
	}
    /if (${Target.ID}) /squelch /alert add 25 id ${Target.ID}
    /if (!${CorpseRotTimer}) /varset CorpseRotTimer ${CorpseRotTime}
}
:clickdoneffs
/if (${CorpseID}) /squelch /alert add 25 id ${CorpseID}
/nomodkey /notify LootWnd LW_DoneButton leftmouseup
/delay 1s !${Corpse.Open}
/if (${Corpse.Open}) /goto :clickdoneffs
/return

Sub LootItem(int i,DoWhat,WhichButton)
/declare CorpseItemID int local ${Corpse.Item[${i}].ID}
/nomodkey /shiftkey /itemnotify ${Corpse.Item[${i}].InvSlot} ${WhichButton}mouseup
/delay 5s ${Window[ConfirmationDialogBox].Open} || !${Corpse.Item[${i}].NoDrop}
/if (${Window[ConfirmationDialogBox].Open}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
/delay 5s ${Cursor.ID} || ${WhichButton.NotEqual[left]}
/if (${DoWhat.Equal[Destroy]} && ${Cursor.ID}==${CorpseItemID}) /destroy
/delay 3s !${Corpse.Item[${i}].ID}
/if (${Cursor.ID}) /call CheckCursor
/return

Sub AdvLootMobs
/declare CurrIni string local
/declare CurrPersonalIni string local
/declare CurrQuestIni string local
/declare CurrGeneralIni string local
/declare CurrentItem string local
/declare i int local
/if (!${AdvLoot.SCount} && !${AdvLoot.PCount} ) {
	/return
} else /if (!${AdvLoot.SCount} && ${AdvLoot.PCount} ) {
	/goto :PersonalCheck
}
/for i ${AdvLoot.SCount} downto 1
	/varset CurrentItem ${AdvLoot.SList[${i}].Name}
	/varset CurrIni ${Ini["loot.ini","${CurrentItem.Left[1]}","${CurrentItem}"]}	
	/varset CurrPersonalIni ${Ini["loot.ini","${Me}","${CurrentItem}"]}
	/varset CurrQuestIni ${Ini["loot.ini","Quests","${CurrentItem}"]}
	/varset CurrGeneralIni ${Ini["loot.ini","General Loot","${CurrentItem}"]}
	/if (${CurrIni.Left[5].Equal[Keep|]} && ${FindItemCount[${CurrentItem}]}>=${CurrIni.Arg[2,|]}) /next i
	/if (${CurrPersonalIni.Left[5].Equal[Keep|]} && ${FindItemCount[${CurrentItem}]}>=${CurrPersonalIni.Arg[2,|]}) {
		/echo ${CurrPersonalIni.Arg[2,|]} is the max i can loot for ${CurrentItem}
		/advloot shared ${i} leave
		/goto :nextitem
	}
	/if (${CurrGeneralIni.Left[5].Equal[Keep|]} && ${FindItemCount[${Corpse.Item[${i}]}]}>=${CurrGeneralIni.Arg[2,|]}) {
		/echo ${CurrGeneralIni.Arg[2,|]} is the max i can loot for ${CurrentItem}
		/advloot shared ${i} leave
		/goto :nextitem
	}
	/if (${CurrIni.Equal[Destroy]}) {
		/if (!${QuestOnly}) /advloot shared ${i} giveto ${Me.Name}
		/goto :nextitem
	}
	/if (${CurrIni.Equal[Ignore]}) {
		/advloot shared ${i} leave
		/goto :nextitem
	}
	/if (!${LootTrash}) /goto :personal
 	|/echo ${CurrentItem} is worth ${Math.Calc[${DisplayItem[${CurrentItem}].Value}/1000]}pp
	/if (!${Select[${CurrIni},Ignore,Keep,Destroy,Sell,Buy,${Me.Name}]} && !${CurrIni.Find[Quest|]} && !${CurrIni.Left[5].Equal[Keep|]} && !${CurrQuestIni.Find[Quest|]} ) {
		/if (${AdvLoot.SList[${i}].NoDrop}) /ini "loot.ini" "${CurrentItem.Left[1]}" "${CurrentItem}" Ignore
		/if (${FindItem[=${CurrentItem}].Value}<${LootValue} && !${AdvLoot.SList[${i}].NoDrop} && !${Select[${CurrIni},Ignore,Keep,Destroy,Quest,Sell]} && ${CurrIni.Left[5].Equal[NULL]}) /ini "loot.ini" "${CurrentItem.Left[1]}" "${CurrentItem}" Ignore
		/if (!${AdvLoot.SList[${i}].NoDrop} && !${Select[${CurrIni},Ignore,Keep,Destroy,Quest,Sell]} && ${CurrIni.Left[5].Equal[NULL]}) /ini "loot.ini" "${CurrentItem.Left[1]}" "${CurrentItem}" Keep
		/varset CurrIni ${Ini["loot.ini","${CurrentItem.Left[1]}","${CurrentItem}"]}
	}
	/if ((${Select[${CurrIni},Keep,Sell]} && (!${FindItem[=${CurrentItem}].Lore} || !${FindItem[=${CurrentItem}].ID}) && ${Me.FreeInventory}) || (${CurrIni.Left[5].Equal[Keep|]}) || (${Me.FreeInventory} && ${FindItem[=${CurrentItem}].Collectible}) || (${AllTradeSkillItems} && ${Me.FreeInventory} && ${FindItem[=${CurrentItem}].Tradeskills})) {
		/advloot shared ${i} giveto ${Me.Name}
	} 
	/if (${CurrIni.Equal[Destroy]}) /advloot shared ${i} giveto ${Me.Name}
	:personal 
	:nextitem
/next i
/delay 1
:PersonalCheck
/if (${AdvLoot.PCount}) {
	/for i ${AdvLoot.PCount} downto 1
		/varset CurrentItem ${AdvLoot.PList[${i}].Name}
		/varset CurrIni ${Ini["loot.ini","${CurrentItem.Left[1]}","${CurrentItem}"]}
		/if (${FindItem[${CurrentItem}].Lore}) {
			/advloot personal ${k} leave
			/goto :NextPersonalItem
		}
		/if (${Select[${CurrIni},Keep,Sell,Destroy]}) {
			/advloot personal ${i} loot
			/delay 5
			/if (${Window[ConfirmationDialogBox].Open}) {
				/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
				/delay 5
			}
			/if (${Select[${CurrIni},Destroy]}) {
				/call DestroyItem "${CurrentItem}" 
			}
		}
		:NextPersonalItem
	/next i
}
/return

Sub DestroyItem(ItemName)
/if (${FindItem[=${ItemName}].ID}) {
	/nomodkey /shift /itemnotify in Pack${Math.Calc[${FindItem[=${ItemName}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${ItemName}].ItemSlot2}+1]} leftmouseup
	/delay 1s ${Cursor.ID}
	/if (${Cursor.ID}==${FindItem[=${ItemName}].ID}) {
		/destroy          
		/delay 1s !${Cursor.ID}
		/echo Destroyed ${ItemName}
	}
}
/return
Sub LootMobs
/varset MacroState Looting Mobs.
/if (${Me.UseAdvancedLooting} && (${Raid.MasterLooter.ID}==${Me.ID} || ${Group.MasterLooter.ID}==${Me.ID})) {
	/call AdvLootMobs
	/if (${RegularLootTimer}) /return
}
/declare i int local
/if (${LootTimer}) /return
/if (!${RegularLootTimer}) /Call CreateTimer RegularLootTimer 5m  
|/if (${NearestSpawn[1,untargetable radius 100 noalert ${AlertList}].ID}) /squelch /alert add ${AlertList} id ${NearestSpawn[1,untargetable radius 100 noalert ${AlertList}].ID}
/if (!${LootMobs} || ${SpawnCount[npc radius ${MobsTooClose} zradius 10 noalert ${AlertList} targetable]} || !${SpawnCount[npccorpse radius ${CorpseRadius} zradius 100 noalert 25]} || ${Me.Combat} || (${Cursor.NoDrop} && !${Me.FreeInventory})) /return
/if (!${CorpseRotTimer}) /squelch /alert clear 25

   /declare CorpseID int local
   /declare StartLoc string ${Me.Loc}
   :NLM_Next
      /varset CorpseID ${Spawn[npccorpse radius ${CorpseRadius} zradius 100 noalert 25].ID}
      /if (!${CorpseID}) {
         /moveto loc ${StartLoc}
         /return
      }
      /if (${Target.ID}!=${CorpseID}) /target id ${CorpseID}
      /if (!${Me.Standing}) /stand
      /delay 1s ${Target.ID}==${CorpseID} && ${Me.Standing}
      /if (${Target.ID}==${CorpseID}) {
         /if (${Spawn[id ${CorpseID}].Distance}>5) /stick 5
         /delay 3s ${Spawn[id ${CorpseID}].Distance}<=15
         /if (${Spawn[id ${CorpseID}].Distance}<15) /call LootCorpse
         /squelch /stick off
      }
	  /if (!${LootMobs} || ${SpawnCount[npc radius ${MobsTooClose} zradius 10 noalert ${AlertList} targetable]} || !${SpawnCount[npccorpse radius ${CorpseRadius} zradius 10 noalert 25]} || ${Me.Combat} || (${Cursor.NoDrop} && !${Me.FreeInventory})) /return
   /goto :NLM_Next

/return

Sub Event_ALCantLoot
/varset ALDontLoot ${Target.ID}
/return

Sub Event_SomeoneElse
/return SomeoneLooting

Sub CheckCursor
:ALauto_inv
/if (${Cursor.ID}) {
  /if (${Cursor.NoDrop} && !${Me.FreeInventory}) /return
  /timed 5 /autoinventory
  /delay 2s !${Cursor.ID}
  /goto :ALauto_inv
}
/return

Sub Event_CurrentInventoryProfit
/declare ItemName string local
/declare Slot int local
/declare Item int local
/declare IniValue string local
/declare Profit int local 0
/for Slot 23 to 32
	/if (${Me.Inventory[${Slot}].Container}) {
		/for Item 1 to ${Me.Inventory[${Slot}].Container}
			/varset ItemName ${Me.Inventory[${Slot}].Item[${Item}].Name}
			/if (${Bool[${ItemName}]}) {
				/varset IniValue ${Ini["loot.ini","${ItemName.Left[1]}","${ItemName}"]}
				/if (${IniValue.Equal[Sell]}) {
					/varcalc Profit ${Profit} + (${FindItem[${ItemName}].Value}*${Me.Inventory[${FindItem[${ItemName}].ItemSlot}].Item[${FindItem[${ItemName}].ItemSlot2}].Stack})
				}
			}
		/next Item
	} else {
		/varset ItemName ${Me.Inventory[${Slot}].Item[${Item}].Name}
		/if (${Bool[${ItemName}]}) {
			/varset IniValue ${Ini["loot.ini","${ItemName.Left[1]}","${ItemName}"]}
			/if (${IniValue.Equal[Sell]}) {
				/varcalc Profit ${Profit} + (${FindItem[${ItemName}].Value}*${Me.Inventory[${FindItem[${ItemName}].ItemSlot}].Item[${FindItem[${ItemName}].ItemSlot2}].Stack})
			}
		}
	}
/next Slot
/echo Current Inventory Profit: ${Math.Calc[${Profit}/1000].Int} Platinum 
/return