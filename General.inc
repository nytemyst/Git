#event IRC				"<#1#> #2#"
#event IRC				"[#1#(msg)] #2#"
#event ExcludeMob		"[MQ2] exclude: #1#"
#event NamedMob			"[MQ2] named: #1#"
#event MezzImmuneMob	"[MQ2] MezzImmune: #1#"
#event Zone				"You have entered #1#."
#event Zone				"LOADING, PLEASE WAIT..."
#event DoTell 			"#1# tells you, '#2#'"
#event DoTell 			"#1# told you, '#2#'" 
#event Experience		"#*#You have slain#*#"
#event Experience		"#*#has been slain by#*#"
#event Experience		"#*#You gain party experience#*#"
#event Experience		"#*#You gained raid experience#*#"
#event Dead				"#*#Returning to Bind Location#*#"
#event Dead				"You have been slain by#*#"
#event Dead				"You died."
#event SetVar 			"[MQ2] set: #1# #2# #3#"
#event GOM				"#*#you are granted a gift#*#"
Sub GeneralDeclares2
   /echo Setting up all the common Variables.
   /if (!${Defined[castReturn]}) /declare castReturn string outer CAST_CANCELLED
   /declare GeneralVersion float  outer 1.50
   /declare noInvis        int    outer 1
   /declare follname	   string outer
   /declare MacroState     string outer
   /declare ExcludeList    string outer
   /declare MezzImmuneList string outer
   /declare NameList       string outer
   /declare RangeMobList   string outer
   /declare LeashTag       int    outer 0
   /declare stakeX         int    outer ${Me.X}
   /declare stakeY         int    outer ${Me.Y}
   /declare MaID           int    outer
   /declare MATarget       int    outer 0
   /declare currentY       float  outer
   /declare currentX       float  outer
   /declare ExcludeIni     string outer AF_ignore.ini
   /declare MezzImmuneIni  string outer AF_MezzImmune.ini
   /declare NameIni        string outer AF_Names.ini
   /declare RangeMobsIni   string outer AF_Rangemobs.ini
   /declare currentZone    int    outer 0
   /declare TankList       string outer Shadow Knight|Paladin|Warrior
   /declare BuffList       string outer Enchanter|Shaman|Cleric|Druid|Magician
   /declare CureList       string outer Shaman|Cleric|Druid|
   /declare MeleeList      string outer Warrior|Rogue|Beastlord|Monk|Shadow knight|Paladin|Berserker|Bard|Ranger|
   /declare RaidBuffList   string outer Beastlord|Paladin|Ranger|Cleric|Druid|Shaman|Enchanter|Wizard|
   /declare CanCast		   string outer Beastlord|Shadow knight|Paladin|Ranger|Cleric|Druid|Shaman|Enchanter|Wizard|Necromancer|Magician|
   /declare CheckExpTimer  timer  outer 0
   /declare MemSpellTimer  timer  outer 0
   /declare RodList        string outer |Summoned: Large Modulation Shard|
   /declare BcClients 	   string outer
   /declare BcClients2 	   string outer
   /declare Died		   bool   outer FALSE
   /declare MacroName      string outer ${Macro.Name}
   /declare CancelledToHeal  bool   outer FALSE	
   /declare FireImmuneList	string outer |Yymp the Abyssal|WildFire|Actinic Fire|Spreading Fire|Oxide Fire|Slave Driver Sedros|Vyzh`dra the Godslayer|Kijarl, Arcanist of Rulnavis|
   /declare IceImmuneList	string outer |Velishan, The Deceitful|Molar|impacted molar|
   /declare MagicImmuneList string outer |Molar|Impacted Molar|Kilsev the Brute|
   /declare PoisonImmuneList string outer |Molar|Impacted Molar|
   /declare DiseaseImmuneList string outer |Molar|Impacted Molar|Xulous Prime|
   
   /call 	DeclareIniVar  DoLeashToon	 int	GeneralSettings 0 	/ltoon
   /call 	DeclareIniVar  DoLeashToonName 	string 	GeneralSettings "bobby" /lperson
   /call	DeclareIniVar  DoLeash       int GeneralSettings 1 /leash 
   /call    DeclareIniVar  leashlength   int GeneralSettings 25 /leashlength
   /call    DeclareIniVar  AutoExpAdjust int GeneralSettings 1 /autoadjustexp
   /call    DeclareIniVar  MaintExpLvl   int GeneralSettings 40 /maintexplvl
   /call    DeclareIniVar  MaxLevel      int GeneralSettings 100 
   /call    DeclareIniVar  MasterList    string GeneralSettings "|"
   /call    DeclareIniVar  autosit       int    GeneralSettings 1 /autosit
   /call    DeclareIniVar  RelayTells    int    GeneralSettings 0 /relay
   /call    DeclareIniVar  DoFood        int    GeneralSettings 0 /dofood
   /Call    DeclareIniVar  EventsMaster  string GeneralSettings "Bobby"
   /call 	DeclareIniVar  UseIRC 		 string GeneralSettings 0   
   /call 	DeclareIniVar  ChatVerbose   Bool	GeneralSettings TRUE	
   /call 	DeclareIniVar  Debug		 Bool	GeneralSettings FALSE
   /call	CleanUpXTarget
   /if (!${CureList.Find[${Me.Class.Name}]}) /call    DeclareIniVar  AutoBalance   int    GeneralSettings 0 /autobalance
   /if (!${CureList.Find[${Me.Class.Name}]}) /call    DeclareIniVar  AutoBalanceHPCheck int GeneralSettings 5 /autobalancehp
   /if (${CureList.Find[${Me.Class.Name}]})  /call    DeclareIniVar  ProtectOutsideGroup bool GeneralSettings false /protectoutside
   /if (${CureList.Find[${Me.Class.Name}]})  /call    DeclareIniVar  ProtectInsideRaid bool GeneralSettings false /ProtectInsideRaid
   /if (${CureList.Find[${Me.Class.Name}]})  /call    DeclareIniVar  ProtectNPC bool GeneralSettings false /protectnpc
   /if (${CureList.Find[${Me.Class.Name}]} && ${ProtectOutsideGroup})  /call    DeclareIniVar  ProtectOutsidePlayersList string HealingTweaks "|bobby|camille|joe|"
   /if (${CureList.Find[${Me.Class.Name}]} && ${ProtectInsideRaid})  /call    DeclareIniVar  ProtectInsideRaidPlayersList string HealingTweaks "|bobby|camille|joe|"
   /if (${CureList.Find[${Me.Class.Name}]} && ${ProtectNPC})  /call    DeclareIniVar  ProtectNPCList string HealingTweaks "|bobby|camille|joe|"
   /squelch /alias /exclude /echo exclude:
   /squelch /alias /named /echo named:
   /squelch /alias /MezzImmune /echo MezzImmune:
   /return

Sub TurkeyMilk
/varset MacroState TurkeyMilk
/declare AmountOfTurkey int local
/varset AmountOfTurkey ${FindItemCount[Cooked Turkey]}
/declare AmountOfMilk int local
/varcalc AmountOfMilk ${FindItemCount[Warm Milk]}-1

/if ( ${AmountOfMilk} <= 5 && ${FindItem[Warm Milk Dispenser].ID} && !${FindItem[Warm Milk Dispenser].Timer} ) {
/call cast "Warm Milk Dispenser" item
/delay 2s ${Me.Casting.ID}
/delay 2s !${Me.Casting.ID}
/delay 1s ${Cursor.ID}
/autoinventory
}

/if ( ${AmountOfTurkey} <= 5 && ${FindItem[Endless Turkeys].ID} && !${FindItem[Endless Turkeys].Timer} ) {
/call cast "Endless Turkeys" item
/delay 2s ${Me.Casting.ID}
/delay 2s !${Me.Casting.ID}
/delay 1s ${Cursor.ID}
/autoinventory
}
/return

Sub AutoAdjustExp
   /varset CheckExpTimer 60s
   /if (${Me.Level} < ${MaxLevel} || (${Me.Level} == ${MaxLevel} && ${Me.PctExp} < ${MaintExpLvl})) {
      /alt off
   }
   /if (${Me.Level} == ${MaxLevel} && ${Me.PctExp} >= ${MaintExpLvl}) {
      /alt on 100
   }
  | /if (${Me.AltAbilityReady[Glyph of Indeterminable Reward]}) /call CastSpell "Glyph of Indeterminable Reward" alt 0
   /return

Sub Leash
/varset MacroState Leash
/doevents Zone
/if (${DoLeashToon} && ${Spawn[pc ${DoLeashToonName}].ID} && !${Spawn[pc ${DoLeashToonName}].Dead}) /return
  /if (${Math.Distance[${Me.Y},${Me.X}:${stakeY},${stakeX}]}>${leashlength} && ${currentZone} == ${Zone.ID}) {
    :FollowingLoop1
    /doevents follow
	/doevents stop
    /if (${Me.Dead}) /return
    /varset currentX ${Me.X}
    /varset currentY ${Me.Y}
    /squelch /face fast nolook loc ${stakeY},${stakeX}
    /if (${Math.Distance[${Me.Y},${Me.X}:${stakeY},${stakeX}]}>10) {
      /moveto loc ${stakeY} ${stakeX}
      /delay 2 
      /if ((${Me.X}==${currentX}) && (${Me.Y}==${currentY})) /call Obstacle
    }
    /if (${Math.Distance[${Me.Y},${Me.X}:${stakeY},${stakeX}]}<=10) /keypress back
    /if (${Math.Distance[${Me.Y},${Me.X}:${stakeY},${stakeX}]}>10) {
      /goto :FollowingLoop1
    } else {
      /keypress forward
      /keypress back
      |/squelch /face fast nolook loc ${stakeY},${stakeX}
      }
    }
  }
  /return

Sub Obstacle
   /varset MacroState Obstacle
   /doevents
   /if (${Died} || ${Me.Dead} || ${Window[ConfirmationDialogBox]}) /return
   /echo Obstacle detected, moving around it
   /keypress forward
   /keypress back hold
   /delay 3
   /keypress back
   /if (${Math.Rand[100]}+1>50) {
      /keypress strafe_right hold
   } else {
      /keypress strafe_left hold
   }
   /delay 2
   /keypress strafe_right
   /keypress strafe_left
   /keypress forward hold
   /keypress jump
   /return

Sub Event_Zone
/declare x int local
   /if ( ${MA.NotEqual[Somebody]} && ${Spawn[pc ${MA}].ID} ) {
      /varset MaID ${Spawn[pc ${MA}].ID}
   } else {
      /varset MaID ${Target.ID}
   }
   /if (${Defined[mobList]}) /call UpdateMobList
   /if (${Defined[addList]}) /varset addList
   /if (${Defined[buffQueue]}) /varset buffQueue
   /if (${Defined[mobID]}) /varset mobID 0
   /if (${Defined[petID]}) /varset petID 0
   /if (${Defined[petID2]}) /varset petID2 0
   /varset currentZone ${Zone.ID} 
   /varset LeashTag 0
   /varset stakeX ${Me.X}
   /varset stakeY ${Me.Y}
   /varset ExcludeList ${Ini[${ExcludeIni},"${Zone.ShortName}","Exclude"]}
   /if (${ExcludeList.Equal[NULL]}) {
      /ini "${ExcludeIni}" "${Zone.ShortName}" "Exclude" "|"
      /varset ExcludeList ${Ini[${ExcludeIni},"${Zone.ShortName}","Exclude"]}
      /echo ExcludeList: ${ExcludeList}
   } else {
      /varset ExcludeList ${Ini[${ExcludeIni},"${Zone.ShortName}","Exclude"]}
      /echo ExcludeList: ${ExcludeList}
   }
   /varset MezzImmuneList ${Ini[${MezzImmuneIni},"${Zone.ShortName}","MezzImmune"]}
   /if (${MezzImmuneList.Equal[NULL]}) {
      /ini "${MezzImmuneIni}" "${Zone.ShortName}" "MezzImmune" "|"
      /varset MezzImmuneList ${Ini[${MezzImmuneIni},"${Zone.ShortName}","MezzImmune"]}
      /echo MezzImmuneList: ${MezzImmuneList}
   } else {
      /varset MezzImmuneList ${Ini[${MezzImmuneIni},"${Zone.ShortName}","MezzImmune"]}
      /echo MezzImmuneList: ${MezzImmuneList}
   }
   /varset NameList ${Ini[${NameIni},"${Zone.ShortName}","Name"]}
   /if (${NameList.Equal[NULL]}) {
      /ini "${NameIni}" "${Zone.ShortName}" "Name" "|"
      /varset NameList ${Ini[${NameIni},"${Zone.ShortName}","Name"]}
      /echo NameList: ${NameList}
   } else {
      /varset NameList ${Ini[${NameIni},"${Zone.ShortName}","Name"]}
      /echo NameList: ${NameList}
   }
   /if (${MeleeList.Find[${Me.Class.Name}]}) {
   /varset RangeMobList ${Ini[${RangeMobsIni},"${Zone.ShortName}","Name"]}
    /if (${RangeMobList.Equal[NULL]}) {
      /ini "${RangeMobsIni}" "${Zone.ShortName}" "Name" "|"
      /varset RangeMobList ${Ini[${RangeMobsIni},"${Zone.ShortName}","Name"]}
      /echo RangeMobList: ${RangeMobList}
   } else {
      /varset RangeMobList ${Ini[${RangeMobsIni},"${Zone.ShortName}","Name"]}
      /echo RangeMobList: ${RangeMobList}
   }
 }
 /squelch /alert clear 200
/if (${ExcludeList.Find[|]} && ${Math.Calc[${ExcludeList.Count[|]}-1]} ) {
	/for x 1 to ${Math.Calc[${ExcludeList.Count[|]}-1]}
		/squelch /alert add 200 ${ExcludeList.Arg[${x},|]}
	/next x		
}
   /return

Sub Follow(string line,string sender,string name)
/varset MacroState Follow
/if ( ${sender.Equal[${Me.Name}]} ) /return
/squelch /target clear
/if (!${Spawn[${sender}].ID}) {
      /e ${sender} is not in zone for me to follow!
      /return
}
/if (${DoLeash}) {
      /varset DoLeash 0
      /varset LeashTag 1 | this lets us know leash was active prior to a follow command
      /echo stopped leashing
   }
   /target clear
   /delay 2s !${Target.ID}
/varset follname ${sender}
/varset following 1
/if (${AdvPath.Active} && ${UseAdvPath}) /goto :advpath
   :targetfoll
   /target pc ${follname}
   /delay 2s ${Target.Name.Equal[${follname}]}
   /if (!${Me.Standing}) /stand
   /if (${Target.Name.Equal[${follname}]}) {
      /goto :Loop
   } else {
   /echo going to :targetfoll
      /goto :targetfoll
      :Loop
      /varset currentX ${Me.X}
      /varset currentY ${Me.Y}
      /face fast
     | /if (${Target.Distance}>10) /keypress forward hold
	  /if (${Target.Distance}>10 && !${Stick.Active}) /stick 10
     | /if (${Target.Distance}<9) /keypress back
	  /if (${Target.Distance}<9) /stick off
      /if (!${Target.ID}) {
	  /varset following 0
	  /if (${DoLeash}) /varset DoLeash 0
	  }
      /doevents IRC
    |**  /if (${Target.Distance}>25) {
       /keypress forward hold
       /delay 2
       /if ((${Me.X}==${currentX}) && (${Me.Y}==${currentY})) /call Obstacle
       } **|
      /if (${Target.Name.Equal[${follname}]} && ${following}) {
         /goto :Loop
      } else {
         /keypress forward
        /keypress back
         /return
      }
   }
   :advpath
   /target pc ${follname}
   /delay 1s ${Target.Name.Equal[${follname}]}
   :loop2
   /if (!${AdvPath.Following}) /squelch /afollow
   /doevents
   /if (!${Target.ID}) /varset following 0
   /if ((${Target.Name.Equal[${follname}]})&&(${following}>0)) /goto :loop2
   /return

Sub Stop
/varset following 0
/if (${AdvPath.Following}) {
      /squelch /afollow off
      /keypress forward
      /keypress back
   }
/target clear
/varset stakeX ${Me.X}
/varset stakeY ${Me.Y}
/echo dropping a new leash point (stop)
|/if (${LeashTag}) /leash 1
/return

Sub Event_IRC(string ChatType,string Sender,string ChatText,string Extra)
/if (${MasterList.Find[${Sender}]} && ${ChatText.Left[1].Equal[/]}) /docommand ${ChatText} 
/if (${Window[ConfirmationDialogBox]} && ${ChatText.Find[TakeTL]}) {
	/if (${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find[Grey Hawke]}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
	/return
}
/if (${Window[LargeDialogWindow]} && ${ChatText.Equal[banner]}) {
	/if (${Window[LargeDialogWindow].Child[LDW_TextBox].Text.Find["Would you like to travel"]}) /nomodkey /notify LargeDialogWindow LDW_YesButton leftmouseup 
}
/if (${ChatText.Equal[follow]}) /call Follow  "tell" "${Sender}" "${ChatText}"
/if (${ChatText.Equal[stop]}) {
   	/call Stop "tell" "${Sender}" "{$ChatText}"
	/return
}
/if (${ChatText.Left[5].Equal[Click]}) {
	/Call CastItemClicky "${ChatText.Right[-6]}" 
	/return
}
/if (${ChatText.Equal[Door]}) {
	/declare DoorTimer timer local 4s
	:retrydoortarget
	/if (!${DoorTimer}) /return
	/doortarget 
	/delay 1
	/if (!${Switch.ID}) /goto :retrydoortarget
	/face door nolook
	/delay 1
	:retryopendoor
	/if (!${DoorTimer}) /return
	/if (!${Switch.Open}) {
		/click left door
		/delay 1s
		/goto :retryopendoor
	}
	/return
}
 /if (${Me.Class.Name.Equal[Wizard]} && ${ChatText.Left[6].Equal[Bounce]}) /call Port "${Sender}" "${ChatText.Arg[2, ]}" "${ChatText.Arg[3, ]}" 
   /if (${ChatText.Equal[stopleash]}) /call StopLeash
   /if (${ChatText.Equal[come]} && ${MeleeList.Find[${Me.Class.Name}]}) /call Follow "tell" "${Sender}" "${ChatText}"
   /if (${ChatText.Equal[comehere]} && !${CureList.Find[${Me.Class.Name}]} || ${Me.Class.Name.Equal[Shaman]} && ${ChatText.Equal[comehere]}) /call Follow "tell" "${Sender}" "${ChatText}"
   /if (${ChatText.Equal[cleartarget]} && ${MasterList.Find[${Sender}]} && ${MeleeList.Find[${Me.Class.Name}]}) /multiline ; /target clear; /varset dispelled 0; /varset Snared 0; /varset CurrentTarget 0; /melee reset; /stick off
   /if ( ( ${ChatText.Equal[cleartarget]} ) && ( ${Me.Class.Name.Equal[Necromancer]} || ${Me.Class.Name.Equal[Wizard]} || ${Me.Class.Name.Equal[Magician]} || ${Me.Class.Name.Equal[Druid]} ) ) {
      /if ( ${Me.Pet.ID} ) /squelch /pet back off
      /target clear
	}
   /if (${ChatText.Equal[blunt]} && ${MeleeList.Find[${Me.Class.Name}]}) /bandolier activate blunt
   /if (${ChatText.Equal[dps]} && ${MeleeList.Find[${Me.Class.Name}]}) /bandolier activate dps
   /if (${ChatText.Equal[Invis]} && (${Me.AltAbilityReady[Group Perfected Invisibility]} || ${Me.AltAbilityReady[Group Perfected invisibility To Undead]})) {
     /target ${Me}
	 /delay 1s ${Target.ID} == ${Me.ID}
   	/alt activate 1210
	/if (${Me.Class.Name.Equal[Cleric]}) {
	 /delay 3
	 /alt activate 1212
	}
   	/delay 1s
  }
   /if (${chatText.Equal[charm]} && ${Me.Class.Name.Equal[Enchanter]}) {
      /squelch /assist ${chatSender}
      /delay 5s ${Target.ID}
      /if (${Target.ID}) {
         /call cast  "${charmSpell}" gem10 3s
         /return
      }
   }
   /if (${ChatText.Arg[1, ].Equal[ma]} && ${MeleeList.Find[${Me.Class.Name}]}) {
      /echo Changing MA to ${ChatText.Arg[2, ]}
      /varset MainAssist ${ChatText.Arg[2, ]}
   }
   /if (${ChatText.Equal[attack]} && ${MeleeList.Find[${Me.Class.Name}]}) {
      /multiline ; /attack; /varset validTarget 1; /varset CurrentTarget ${Target.ID}
      /call DPS
      /return
   }
   /if (${ChatText.Left[2].Equal[tl]} && ${Me.Class.Name.Equal[Wizard]}) /call TL "${ChatText.Arg[2, ]}"
   /if (${ChatText.Find[Buff]} && ${BuffList.Find[${Me.Class.Name}]}) {
      /if (${ChatText.Arg[5, ].NotEqual[NULL]}) {
         /call CheckDoBuffs "${ChatText.Arg[2, ]}" "${ChatText.Arg[3, ]} ${ChatText.Arg[4, ]} ${ChatText.Arg[5, ]}"
         /return
      }
      /if (${ChatText.Arg[4, ].NotEqual[NULL]}) {
         /call CheckDoBuffs "${ChatText.Arg[2, ]}" "${ChatText.Arg[3, ]} ${ChatText.Arg[4, ]}"
         /return
      }
      /call CheckDoBuffs "${ChatText.Arg[2, ]}" "${ChatText.Arg[3, ]}"   
      /return
    }
   /if (${Sender.Equal[${Me}]}) /return
   /if (${ChatText.Equal[rc]} && ${CureList.Find[${Me.Class.Name}]}) {
      /if (${Me.AltAbilityReady[radiant cure]}) /call Cast "radiant cure" alt
   }
   /if (${ChatText.Equal[port]} && ${FindItem[fellowship registration insignia].ID}) {
      /echo Going to port away to campfire!
	  /call CastItemClicky "fellowship registration insignia" 
	  /return
   }
   /if (${ChatText.Arg[1, ].Equal[tank]} && ${Me.Class.Name.Equal[cleric]}) {
      /echo Changing Tank to ${ChatText.Arg[2, ]}
      /varset tankname ${ChatText.Arg[2, ]}
      
      /target ${ChatText.Arg[2, ]}
      /varset tanknameid ${Target.ID}
      /varset tankclass ${Target.Class.Name}
   }
   /if (${ChatText.Arg[1, ].Equal[ma]}) {
      /echo Changing MA to ${ChatText.Arg[2, ]}
      /if (${Defined[MainAssist]}) /varset MainAssist ${ChatText.Arg[2, ]}
   }
   /if (${ChatText.Equal[mgb dmf]} && ${Me.AltAbilityReady[Mass Group Buff]} && ${Me.Class.Name.Equal[Necromancer]}) {
      /call cast "Mass group Buff" alt
      /call cast "Dead Men Floating" gem10 3s
   }
   /if (${DoBuffTotal} && ${Sender.NotEqual[${Me.CleanName}]} && ${Spawn[pc ${Sender}].ID} && ${BuffList.Find[${Me.Class.Name}]}) {
     /call CheckDoBuffs "${Sender}" "${ChatText}"
   }
   /if (${ChatText.Equal[CloseWindows]}) {
	/keypress esc
	/keypress esc
	/keypress esc
	/keypress esc
	/keypress esc
	/keypress esc
   }
   /if (${ChatText.Equal[invisoff]}) {
	/makemevisible
   }
/if (${ChatText.Arg[1, ].Equal[burnnow]}) {
	/if (${ChatText.Arg[2, ]}) {
		/Call CreateTimer BurnTimer ${ChatText.Arg[2, ]]}M
	} else {
		/call CreateTimer BurnTimer 5m
	}
}
/if (${ChatText.Arg[1, ].Equal[Burnstop]}) /Call CreateTimer BurnTimer 0
/if (${ChatText.Arg[1, ].Equal[hailon]}) {
	/target npc ${ChatText.Arg[2, ]}
	/delay 1s
	/squelch /hail
}
/if (${ChatText.Arg[1, ].Equal[saythis]}) {
	/target clear
	/target npc ${ChatText.Arg[3, ]}
	/delay 1s
	/if (${Target.ID}) /say ${ChatText.Arg[2, ]}	
}

/if (${ChatText.Arg[1, ].Equal[mgb]}) {
	/if (${ChatText.Arg[2, ].Equal[heal]} && ${Me.Class.Name.Equal[Cleric]} && ${Me.AltAbilityReady[Mass Group Buff]}) {
		/call CastSpell "Mass Group Buff" alt 0
		/delay 5
		/call CastSpell "Celestial Regeneration" alt 0
	} else /if (${ChatText.Arg[2, ].Equal[aid]} && ${Me.Class.Name.Equal[Shaman]} && ${Me.AltAbilityReady[Mass Group Buff]}) {
		/call CastSpell "Mass Group Buff" alt 0
		/delay 5
		/call CastSpell "Ancestral Aid" alt 0
	}
}
/return

Sub Event_ExcludeMob(string line,string MobName)
   /declare CurrentExcludeList string local
   /varset CurrentExcludeList ${Ini[${ExcludeIni},"${Zone.ShortName}","Exclude"]}
   /if (${CurrentExcludeList.Find[NULL]}) {
      /ini "${ExcludeIni}" "${Zone.ShortName}" "Exclude" "${MobName}"
      /varset CurrentExcludeList ${MobName}
   } else /if (!${CurrentExcludeList.Find[${MobName}]}) {
      /ini "${ExcludeIni}" "${Zone.ShortName}" "Exclude" "${ExcludeList}${MobName}|"
      /varset CurrentExcludeList ${Ini[${ExcludeIni},"${Zone.ShortName}","Exclude"]}
   }
   /varset ExcludeList ${CurrentExcludeList}
   /echo ExcludeList: ${ExcludeList}
   /return

Sub Event_NamedMob(string line,string MobName)
   /declare CurrentNameList string local
   /varset CurrentNameList ${Ini[${NameIni},"${Zone.ShortName}","Name"]}
   /if (${CurrentNameList.Find[NULL]}) {
      /ini "${NameIni}" "${Zone.ShortName}" "Name" "${MobName}"
      /varset CurrentNameList ${MobName}
   } else /if (!${CurrentNameList.Find[${MobName}]}) {
      /ini "${NameIni}" "${Zone.ShortName}" "Name" "${NameList}${MobName}|"
      /varset CurrentNameList ${Ini[${NameIni},"${Zone.ShortName}","Name"]}
   }
   /varset NameList ${CurrentNameList}
   /echo NameList: ${NameList}
   /return

Sub Event_MezzImmuneMob(string line,string MobName)
   /declare CurrentMezzImmuneList string local
   /varset CurrentMezzImmuneList ${Ini[${MezzImmuneIni},"${Zone.ShortName}","MezzImmune"]}
   /if (${CurrentMezzImmuneList.Find[NULL]}) {
      /ini "${MezzImmuneIni}" "${Zone.ShortName}" "MezzImmune" "${MobName}"
      /varset CurrentMezzImmuneList ${MobName}
   } else /if (!${CurrentMezzImmuneList.Find[${MobName}]}) {
      /ini "${MezzImmuneIni}" "${Zone.ShortName}" "MezzImmune" "${MezzImmuneList}${MobName}|"
      /varset CurrentMezzImmuneList ${Ini[${MezzImmuneIni},"${Zone.ShortName}","MezzImmune"]}
   }
   /varset MezzImmuneList ${CurrentMezzImmuneList}
   /echo MezzImmuneList: ${MezzImmuneList}
   /return
   
Sub MemSpell(SpellName,SpellGem)
/varset MacroState "MemSpell ${SpellName}"
/if (${MemSpellTimer} || !${Me.Book[${SpellName}]}) {
	/if (!${Me.Book[${SpellName}]} && !${SpellTimer}) {
	/call CreateTimer SpellTimer 60s
	/Echo You don't have ${SpellName} in your Spell book. Check your ini for correct spelling.
	}
	/return
}
/if (${SpellGem.Length}==1)	/memspell ${SpellGem.Right[1]} "${SpellName}"
/if (${SpellGem.Length}==2)	/memspell ${SpellGem.Right[2]} "${SpellName}"
/if (${SpellGem.Length}==4)	/memspell ${SpellGem.Right[1]} "${SpellName}"
/if (${SpellGem.Length}==5)	/memspell ${SpellGem.Right[2]} "${SpellName}"
/delay 5s ${Me.Gem[${SpellName}]}
/delay 3
/varset  MemSpellTimer ${Math.Calc[${Spell[${SpellName}].RecastTime}+12]}S
/return

Sub MedTime
/varset MacroState MedTime
/if (${autosit} && !${Bool[${Me.Sitting}]} && !${Me.Casting.ID} && !${Me.Moving} && !${Me.CombatState.Equal[combat]} && (!${SpawnCount[npc los range 5 110 radius 50 zradius 15 targetable noalert 200]})) {
	/if (${MeleeList.Find[${Me.Class.Name}]} && ${CanCast.Find[${Me.Class.Name}]} && !${SitTimer}) {
		/if (${Me.PctEndurance}<21 && ${Me.CombatAbilityReady[${RespiteDisc}]} && !${Me.CombatState.Equal[Combat]} && !${Me.ActiveDisc.ID}) /disc ${RespiteDisc}
		/if (${Me.PctEndurance}<75 && !${Me.Sitting}) /sit
		/if (${Me.PctMana}<75 && !${Me.Sitting}) /sit
		/call CreateTimer SitTimer 30s
	} else /if (${MeleeList.Find[${Me.Class.Name}]} && !${CanCast.Find[${Me.Class.Name}]} && !${Me.Casting.ID} && ${Me.PctEndurance}<75 && !${SitTimer}) {
      	/if (${Me.PctEndurance}<21 && ${Me.CombatAbilityReady[${RespiteDisc}]} && !${Me.CombatState.Equal[Combat]}) /disc ${RespiteDisc}
         /sit
		 /call CreateTimer SitTimer 30s
	} else /if (!${MeleeList.Find[${Me.Class.Name}]} && ${CanCast.Find[${Me.Class.Name}]} && !${Me.Casting.ID} && ${Me.PctMana}<75) {
         /sit
		 /call CreateTimer SitTimer 30s
	}
}
/if (${MeleeList.Find[${Me.Class.Name}]} && ${CanCast.Find[${Me.Class.Name}]} && ${Me.PctEndurance}>99 && ${Me.PctMana}>99 && !${Window[SpellBookWnd].Open}) {
      /stand
} else /if (${MeleeList.Find[${Me.Class.Name}]} && !${CanCast.Find[${Me.Class.Name}]} && ${Me.PctEndurance}>99) {
      /stand
} else /if (!${MeleeList.Find[${Me.Class.Name}]} && ${CanCast.Find[${Me.Class.Name}]} && ${Me.PctMana}>99 && !${Window[SpellBookWnd].Open}) {
      /stand
}
/return

Sub Event_DoTell(string line,string person, string said)
/if (${RelayTells} && !${line.Find[${Me}]}) {
	/docommand /bc ${person} told Me ${said}
	/beep
}
/return

Sub ReturnAutoBalance
/varset MacroState Autobalance
/declare Balanced bool local TRUE
/if (!${Defined[IniLoaded]}) /declare IniLoaded bool outer FALSE
/if (!${Defined[NameCount]}) /declare NameCount int outer ${Ini[af_balance.ini,Mobs,NameCount]}
/declare Mobs[${NameCount}] string local
/declare x int local
/declare b int local 0
/declare lowesthp int local 100
/declare lowestmob int local
/declare MyTarget  int local
|Load the entire ini into Memory
/for x 1 to ${NameCount}
		/varset Mobs[${x}] ${Ini[af_balance.ini,Mobs,Name${x}]}
/next x
|Find how many are in the zone to balance and set them to a variable.
/for x 1 to ${NameCount}
	/if (${Spawn[npc ${Mobs[${x}]}].ID} && !${Defined[BalanceMob${Math.Calc[${b}+1].Int}]}) { 
			/declare BalanceMob${Math.Calc[${b}+1].Int} int outer ${Spawn[npc ${Mobs[${x}]}].ID}
			/varcalc b ${b}+1
			/echo Found ${Spawn[npc ${Mobs[${x}]}].CleanName} to balance
			} else /if (${Defined[BalanceMob${Math.Calc[${b}+1].Int}]}) {
				/varcalc b ${b}+1
			}
/next x
/if (${b} <= 1) /return
|Let's Find the NPC that has the lowest Health and which mob we're currently on.
/for x 1 to ${b}
/if (${Spawn[npc id ${BalanceMob${x}}].PctHPs} < ${lowesthp}) {
	/varset lowestmob ${BalanceMob${x}} 
	/varset lowesthp ${Spawn[npc id ${BalanceMob${x}}].PctHPs}
	}
/if (${Target.ID} == ${BalanceMob${x}}) /varset MyTarget ${BalanceMob${x}}
/next x
|If we're on the lowest mob then let's make sure the others are caught up if not stop. 
|If we're not on the lowest mob then let's continue until we're the lowest mob and it's unbalanced.
/if (${MyTarget} == ${lowestmob}) {
	/for x 1 to ${b}
	/if (${BalanceMob${x}} == ${MyTarget}) /next x
	/if (${Math.Abs[${Math.Calc[${Spawn[npc id ${BalanceMob${x}}].PctHPs} - ${Spawn[npc id ${MyTarget}].PctHPs}]} >= ${AutoBalanceHPCheck}]}) {
		/varset Balanced FALSE
		/return
		}
/next x
} 
|If we got here then all mobs are balanced or you need to catch up with the rest.
/varset Balanced TRUE
/return ${Balanced}

Sub SplashCast(spellName,spellType,giveUpValue,mySub,int ResistTotal)
/delay 2s ${Me.SpellReady[${spellName}]}
 /nomodkey /cast "${spellName}"
 /face fast id ${Target.ID}
 /delay 5
 /click left center
 /delay 2s !${Me.Casting.ID}
/return

Sub PotionCast(string ItemName, int Slot)
/varset MacroState PotionCast
/if ( !${Defined[CastSlot]} ) /declare CastSlot int local ${Slot}
/varcalc Slot (${Slot}-1)
/if ( ${Window[PotionBeltWnd].Child[PW_PotionSlot${Slot}_Button].Tooltip.Equal[${ItemName}]} && !${Bool[${FindItem[${ItemName}].Timer}]} && ${Bool[${FindItem[${ItemName}]}]}) /potionbelt Activate ${CastSlot}
/return

Sub CastItemClicky(string sItemClicky)
/varset MacroState CastItemClicky
/if (!${FindItem[=${sItemClicky}].ID}) {
	/if (!${ItemTimer}) {
		/call CreateTimer ItemTimer 60s
		/echo You don't seem to have ${sItemClicky} in your Inventory. Check inventory and Ini Spelling.
	}
	/return
}
/if (!${FindItem[=${sItemClicky}].Timer}) {	
	/if (!${me.Casting.ID} || ${Me.Class.Name.Equal[Bard]}) {
		/useitem ${FindItem[=${sItemClicky}].ItemSlot} ${FindItem[=${sItemClicky}].ItemSlot2}
		/Echo Using ${sItemClicky} Clicky
		/delay ${Math.Calc[${FindItem[${sItemClicky}].CastTime}+1]}s
	}
} 
/return

Sub CheckBuffies(string MySub)
/declare a int local
/for a 1 to ${NumSelfBuff}
    /if ( ${selfBuff${a}.NotEqual[NULL]} && ${SelfType${a}.Equal[item]} && ${FindItem[=${selfBuff${a}}].ID} && ${Me.Buff[${FindItem[=${selfBuff${a}}].Spell.Name}].Duration}<4 && !${FindItem[=${selfBuff${a}}].Timer}) {
       /if (!${Spell[${FindItem[=${selfBuff${a}}].Spell.Name}].Stacks[2]}) /goto :SkipBuff
		/call CastItemClicky "${selfBuff${a}}"
    } else /if ( ${Me.Book[${selfBuff${a}}]} && ${SelfType${a}.Find[gem]} && ${selfBuff${a}.NotEqual[NULL]} && ${Spell[${selfBuff${a}}].Mana} < ${Me.CurrentMana} && !${Me.Class.Name.Equal[bard]} || ${SelfType${a}.Find[alt]} ) {
      /if ( !${Me.Buff[${selfBuffToCheck${a}}].ID} || ${Me.Buff[${selfBuffToCheck${a}}].Duration} < 4 ) {
         /if (!${Spell[${selfBuff${a}}].Stacks[2]}) /goto :SkipBuff
		 /if (!${Me.Gem[${selfBuff${a}}]} && !${SelfType${a}.Find[alt]})  /call MemSpell "${selfBuff${a}}" ${SelfType${a}}      
		 /if (${Me.SpellReady[${selfBuff${a}}]} || ${Me.AltAbilityReady[${selfBuff${a}}]}) {
			/if (${Spell[${selfBuff${a}}].TargetType.Find[Single]}) /target myself
			/if (${Bool[${MySub}]}) /call CastSpell "${selfBuff${a}}" "${SelfType${a}}" 0 ${MySub}
			/if (!${Bool[${MySub}]}) /call CastSpell "${selfBuff${a}}" "${SelfType${a}}" 0 
      /return
      }
    }
}
    :SkipBuff
  /next a
  /if (${Cursor.ID} && (${Me.CombatState.Equal[Combat]} || ${Cursor.ID} == 79322)) /autoinventory
  /if (${Window[Itemdisplaywindow].Open} && !${ItemWindowTimer}) {
	/keypress esc
	/Call CreateTimer ItemWindowTimer 12s
  } 
  
/if (${Window[ConfirmationDialogBox]}) {
	/if (${Window[ConfirmationDialogBox].Child[CD_TextOutPut].Text.Find["Join the expedition"]}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup 
	/if (${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find[Blessing of Resurrection]}) /call Wait4rez
}

/if (${Window[Tradewnd].HisTradeReady}) {
	/nomodkey /notify Tradewnd TRDW_Trade_Button leftmouseup
	/delay 1s ${Cursor.ID}
	/autoinv
	/delay 1s !${Cursor.ID}
}
/return


Sub Event_Dead
/varset Died TRUE
/return

||Wait4rez.inc v2.21
Sub Wait4Rez
/varset MacroState Wait4rez
/echo Awaiting rez: auto-accepting, and looting my corpse
:rezmeffs
/delay 10s ${Window[ConfirmationDialogBox].Open}
/if (!${Window[ConfirmationDialogBox].Open}) /goto :rezmeffs
/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
/delay 1
/if (${Window[RespawnWnd].Open}) /if (${Zone.ShortName.Equal[Powar]}) {
	/nomodkey /notify RespawnWnd RW_OptionsList listselect 1
} else {
	/nomodkey /notify RespawnWnd RW_OptionsList listselect 2 
}
/delay 1
/nomodkey /notify RespawnWnd RW_SelectButton leftmouseup
/delay 1
/delay 1s
/call ResetVariables
/call Event_Zone
/echo Rez Successful
/varset Died False
/doevents flush 
/return 

Sub MakeCampfire
|Taken from modbot
/windowstate FellowshipWnd open
/nomodkey /notify FellowshipWnd FP_Subwindows tabselect 2
/delay 1s
/nomodkey /notify FellowshipWnd FP_RefreshList leftmouseup
/delay 1s
/nomodkey /notify FellowshipWnd FP_CampsiteKitList listselect 1
/delay 1s
/nomodkey /notify FellowshipWnd FP_CampsiteKitList leftmouse 1
/delay 1s
/nomodkey /notify FellowshipWnd FP_CreateCampsite leftmouseup
/delay 1s
/windowstate FellowshipWnd close
/return

Sub BCBuff(string MySub)
/declare X int local
/declare b int local 1
/declare ignorelist string local
/varset MacroState BCBuffs
/varset BcClients ${NetBots.Client} 
/varset BcClients2 ${Math.Calc[${BcClients.Count[ ]}+1]}
/call CreateTimer BCBuffTimer 25S
/if (${UseReverseBcOrder}) /goto :Reverse
	/for X 1 to ${BcClients2}
  	/if (!${NetBots[${BcClients.Arg[${X}]}].InZone} || ${Spawn[pc ${NetBots[${BcClients.Arg[${X}]}].Name}].Distance} > 100) /next X	
    /for b 1 to ${DoBcBuffTotal}  
     /varset ignorelist ${Ini[${iniName},"BCBuffs","DoBCBuffIgnoreClass${b}"]}
     /if (!${NetBots[${BcClients.Arg[${X}]}].Buff.Find[${Spell[${DoBCBuff${b}}].ID}]} && ${Me.PctMana}> ${DoBCBuffMana${b}} && !${ignorelist.Find[${NetBots[${BcClients.Arg[${X}]}].Class}]} && ${Me.CurrentMana}>${Spell[${DoBCBuff${b}}].Mana} && ${NetBots[${BcClients.Arg[${X}]}].Stacks[${Spell[${DoBCBuff${b}}].ID}]}) { 
		/call CastSpell "${DoBCBuffCast${b}}" ${BuffGem} ${NetBots[${BcClients.Arg[${X}]}].ID} checkall 	
     	/return     
	}
/next b
/next X
}
/goto :end
:Reverse
/for X ${BcClients2} downto 1
/if (!${NetBots[${BcClients.Arg[${X}]}].InZone} || ${Spawn[pc ${NetBots[${BcClients.Arg[${X}]}].Name}].Distance} > 100) /next X	
    /for b 1 to ${DoBcBuffTotal}  
     /varset ignorelist ${Ini[${iniName},"BCBuffs","DoBCBuffIgnoreClass${b}"]}
     /if (!${NetBots[${BcClients.Arg[${X}]}].Buff.Find[${Spell[${DoBCBuff${b}}].ID}]} && ${Me.PctMana}> ${DoBCBuffMana${b}} && !${ignorelist.Find[${NetBots[${BcClients.Arg[${X}]}].Class}]} && ${Me.CurrentMana}>${Spell[${DoBCBuff${b}}].Mana} && ${NetBots[${BcClients.Arg[${X}]}].Stacks[${Spell[${DoBCBuff${b}}].ID}]}) {     
		/call CastSpell "${DoBCBuffCast${b}}" ${BuffGem} ${NetBots[${BcClients.Arg[${X}]}].ID} checkall 		
     	/return     
	}
:end
/return




Sub Irc
/Call DeclareIniVar IRCServer string GeneralSettings ""
/Call DeclareIniVar IRCChannel string GeneralSettings ""
/if (!${Irc}) {
    /iconnect ${IRCServer} ${IRCCchannel} ${Me}
   /timed 30 /i join #${IRCChannel}
   /varset irctimer 30s
 }
/if (${Irc.Channel.Equal[null]}) {
	/i quit
	/varset irctimer 30s
	}
/return

Sub DeclareIniVar(string name,string varType,string section,string value,string alias)
/if ( !${Defined[${name}]} ) /declare ${name} ${varType} outer
/if (${value.Length}==0) /varset value ChangeMe
/varset ${name} ${Ini[${iniName},${section},${name.Left[1].Upper}${name.Right[-1]},"${value}"]}
/ini "${iniName}" "${section}" "${name.Left[1].Upper}${name.Right[-1]}" "${${name}}"
/if ( (${Defined[alias]} && ${DoAlias}) || ( ${Defined[alias]} && ${Plugin[mq2alias].Name.Length} && !${Alias[${alias}]}) )  /squelch /alias ${alias} /echo set: ${name} ${section}
/if (${Me.Book[${Me.Book[${${name}} rk. iii]}].ID}) /varset ${name} ${Me.Book[${Me.Book[${${name}} rk. iii]}].Name} 
/if (${Me.Book[${Me.Book[${${name}} rk. ii]}].ID}) /varset ${name} ${Me.Book[${Me.Book[${${name}} rk. ii]}].Name}
/if (${Me.CombatAbility[${${name}} rk. iii]}) /varset ${name} ${${name}} rk. iii
/if (${Me.CombatAbility[${${name}} rk. ii]}) /varset ${name} ${${name}} rk. ii
/return

Sub Event_SetVar(string line,string varName,string section,string value)
/if ( ${value.Equal[on]} ) /varset value 1
/if ( ${value.Equal[off]} ) /varset value 0
/if ( ${value.Equal[TRUE]} ) /varset value TRUE
/if ( ${value.Equal[FALSE]} ) /varset value FALSE
     /varset ${varName} ${value}
     /popup ${varName} is set to ${value} now.
     /ini "${iniName}" ${section} "${varName.Left[1].Upper}${varName.Right[-1]}" "${value}"
/return

Sub CastSpellx(MySpell,MyType,int MyTarget,MySub)
/varset MacroState CastSpell ${MySpell}
/doevents
/if (${Me.Dead} || ${Died} || ${Me.Casting.ID}) /return
/if (${MyType.Equal[alt]}) {
	/if (!${Me.AltAbilityReady[${MySpell}]}) /return ${MySpell} NOTREADY 
	/if (${Spell[${MySpell}].SpellType.Equal[Self]} || ${MyTarget}==0) {
		/echo Casting AA ${MySpell}
		/if (!${Me.AltAbility[${MySpell}].Spell.MyCastTime}>=0.5) {
			/alt activate ${Me.AltAbility[${MySpell}].ID}
		} else /if (${MySub.NotEqual[NULL]} ) {
			/call Cast "${MySpell}" ${MyType} 0 ${MySub}
		} else /call Cast "${MySpell}" ${MyType} 0 
	/return
	}
} else /if (${MyType.Equal[Disc]]}) {
	/if (!${Me.CombatAbilityReady[${MySpell}]} || ${Me.CurrentEndurance} < ${Spell[${MySpell}].EnduranceCost}) /return NOTREADY
	/echo Using Disc ${MySpell} 
	/disc ${MySpell}
	/delay 5 !${Me.CombatAbilityReady[${MySpell}]}
	/return
} else /if (${MyType.Equal[item]}) {
	/if (${FindItem[=${MySpell}].Timer} && ${FindItem[=${MySpell}].ID}) /return NOTREADY
	/if (${Me.Class.Name.Equal[Bard]} && ${Twist}) {
		/squelch /stopswitch
		/delay 1s !${Twist}
	}
	/call CastItemClicky "${MySpell}"
	/return 
} else {
	/if (!${Me.SpellReady[${MySpell}]}) /return NOTREADY
	/if (${Spell[${MySpell}].SpellType.Equal[Self]} || ${MyTarget}==0) {
		
	}
}
/return


Sub CastSpell(MySpell,MyType,int MyTarget,MySub)
/varset MacroState CastSpell ${MySpell}
/doevents
/if (${Me.Casting.ID} && (${MyType.NotEqual[alt]} && ${MyType.NotEqual[item]})) /multiline ; /varset castReturn CAST_NOTREADY;/return 
/if ((${MyType.NotEqual[alt]} && ${MyType.NotEqual[item]} && ${MyType.NotEqual[disc]}) && !${Me.Gem[${MySpell}]}) /call MemSpell "${MySpell}" ${MyType}
/if (!${Me.SpellReady[${MySpell}]} && (!${MyType.Equal[item]} && !${MyType.Equal[alt]} && !${MyType.Equal[disc]}) || !${Me.AltAbilityReady[${MySpell}]} && ${MyType.Equal[alt]} || !${Me.CombatAbilityReady[${MySpell}]} && ${MyType.Equal[disc]} || ${Me.CurrentEndurance} < ${Spell[${MySpell}].EnduranceCost} || ${Me.CurrentMana} < ${Spell[${MySpell}].Mana} && (!${MyType.Equal[alt]} && !${MyType.Equal[item]} && !${MyType.Equal[disc]}) || ${MyType.Equal[item]} && ${FindItem[=${MySpell}].Timer} && ${FindItem[=${MySpell}].ID} || ${Spawn[${MyTarget}].Type.Equal[corpse]}) {
	/varset castReturn CAST_NOTREADY
	|/if (${MyType.Equal[alt]}) /echo ${MySpell} wasn't ready.
	|/if (${Me.Class.Name.Equal[shaman]}) /echo ${MySpell} not ready
	/return ${MySpell} NOTREADY
	}
/if (${MyTarget}==0 || ${Spell[${MySpell}].SpellType.Equal[Self]}) /goto :notarget
/if (!${Spawn[${MyTarget}].ID}) {
	/echo You tried to cast ${MySpell} on something that doesn't exist
	/multiline ; /varset castReturn CAST_NOTREADY;/return 
	}
/if (${Target.ID} != ${MyTarget}) /tar id ${MyTarget}
/delay 1s ${Target.ID} == ${MyTarget}
/if (${Target.Type.Equal[Corpse]} || !${Target.ID} || (!${Target.LineOfSight} && !${Spell[${MySpell}].SpellType.Equal[Beneficial]}) || ${Target.Distance} > 300) /return
:NoTarget
/if (${Target.Type.Equal[Corpse]} || (!${Target.LineOfSight} && !${Spell[${MySpell}].SpellType.Equal[Beneficial]}) || ${Target.Distance} > 300 ) /return 45
/if (${MyType.Equal[item]}) {
	/if (${Me.Class.Name.Equal[Bard]} && ${Twist}) {
		/squelch /stopswitch
		/delay 1s !${Twist}
	}
	/call CastItemClicky "${MySpell}"
	/return 
}
/if (${MyType.Equal[disc]}) {
	/if (${MyTarget}==0 && ${Target.ID}) /echo Using Disc ${MySpell} 
	/disc ${MySpell}
	/delay 5 !${Me.CombatAbilityReady[${MySpell}]}
	/return
}
/if ((${DiseaseImmuneList.Find[${Target.CleanName}]} && ${Spell[${MySpell}].ResistType.Equal[Disease]}) || (${PoisonImmuneList.Find[${Target.CleanName}]} && ${Spell[${MySpell}].ResistType.Equal[Poison]}) || (${FireImmuneList.Find[${Target.CleanName}]} && ${Spell[${MySpell}].ResistType.Equal[Fire]}) || (${IceImmuneList.Find[${Target.CleanName}]} && ${Spell[${MySpell}].ResistType.Equal[Cold]}) || (${MagicImmuneList.Find[${Target.CleanName}]} && ${Spell[${MySpell}].ResistType.Equal[Magic]}) ) {
	/varset castReturn CAST_NOTREADY
	/return Mob Immune to ${MySpell}
}
:waitforspelltofinish
/if (${Died} || ${Me.Dead}) /return
/if (${Me.Casting.ID} && (${MyType.Equal[item]} || ${MyType.Equal[alt]}) && ${Me.Class.Name.NotEqual[Bard]}) /goto :waitforspelltofinish
/if (${MySub.NotEqual[NULL]} ) {
	/if (${Me.Sitting}) /stand
	/if (${MyTarget}!=0) /echo Casting ${MySpell} on ${Spawn[ID ${MyTarget}].CleanName}
	/if (${MyTarget}==0 && ${Target.ID} && ${ChatVerbose}) /echo Casting ${MySpell} 
	/if (${Me.Class.Name.NotEqual[bard]} || ${Me.AltAbility[${MySpell}].Spell.MyCastTime}>=3) {
		/call Cast "${MySpell}" ${MyType} 0 ${MySub}
	} else {
		/alt activate ${Me.AltAbility[${MySpell}].ID}
	}	
	/if (${MyType.Equal[alt]}) /delay 5 !${Me.AltAbilityReady[${MySpell}]}
	/if (!${MyType.Equal[item]} && !${MyType.Equal[alt]} && !${MyType.Equal[disc]}) /delay 5 !${Me.SpellReady[${MySpell}]}
	/return 
} else {
	/if (${Me.Sitting}) /stand
	/if (${MyTarget}!=0 && ${ChatVerbose}) /echo Casting ${MySpell} on ${Spawn[ID ${MyTarget}].CleanName}
	/if (${MyTarget}==0 && ${Target.ID} && ${ChatVerbose}) /echo Casting ${MySpell} 
	/if (${Me.Class.Name.NotEqual[bard]} || ${Me.AltAbility[${MySpell}].Spell.MyCastTime}>=3) {
		/call Cast "${MySpell}" ${MyType} 0 
	} else {
		/alt activate ${Me.AltAbility[${MySpell}].ID}
	}	
	/if (${MyType.Equal[alt]}) /delay 5 !${Me.AltAbilityReady[${MySpell}]}
	/if (!${MyType.Equal[item]} && !${MyType.Equal[alt]} && !${MyType.Equal[disc]}) /delay 5 !${Me.SpellReady[${MySpell}]}
	/return 
}
/return 

Sub CreateTimer(MyTimer,MyLength)
/if (!${Defined[${MyTimer}]} ) /declare ${MyTimer} timer outer
/if (${MyLength.NotEqual[NULL]}) /varset ${MyTimer} ${MyLength}
/return

Sub AddTanks
/declare x int local
/declare b int local 1
/declare GroupList	string local 
/declare TempName	string local
/declare Warriors	int local 
/declare SKS		int local 
/declare Paladins	int local 
/declare Rangers	int local
/declare Zerkers	int local
/declare Wizards	int local
/declare Rogues		int local
/declare Necros		int local
/declare Bards		int local
/declare Monks		int local
/varset Warriors ${SpawnCount[pc warrior raid radius 300]}
/varset SKS ${SpawnCount[pc shadowknight raid radius 300]}
/varset Paladins ${SpawnCount[pc paladin raid radius 300]}
/varset Rangers ${SpawnCount[pc Ranger raid radius 300]}
/varset Zerkers ${SpawnCount[pc Berserker raid radius 300]}
/varset Wizards ${SpawnCount[pc Wizard raid radius 300]}
/varset Rogues ${SpawnCount[pc Rogue raid radius 300]}
/varset Necros ${SpawnCount[pc Necromancer raid radius 300]}
/varset Bards ${SpawnCount[pc Bard raid radius 300]}
/varset Monks ${SpawnCount[pc Monk raid radius 300]}
/call ReturnCurrentGroup
/if (${Me.Class.Name.Equal[shaman]}) /goto :shaman
/if (${Warriors}) {
/for x 1 to ${Warriors}
	/if (${b} >12) /return
	/if (${Macro.Return.Find[${NearestSpawn[${x}, pc warrior raid].Name}]}) /goto :skip1 
	/xtarget set ${b} ${NearestSpawn[${x}, pc warrior raid]}
	/varcalc b ${b}+1
	:skip1
	/next x
	}
/if (${SKS} && ${XHealKnights}) {
/for x 1 to ${SKS}
	/if (${b} >12) /return
	/if (${Macro.Return.Find[${NearestSpawn[${x}, pc Shadowknight raid].Name}]}) /goto :skip2
	/xtarget set ${b} ${NearestSpawn[${x}, pc shadowknight raid]}
	/varcalc b ${b}+1
	:skip2
	/next x
	}
/if (${Paladins} && ${XHealKnights}) {
/for x 1 to ${Paladins}
	/if (${b} >12) /return
	/if (${Macro.Return.Find[${NearestSpawn[${x}, pc paladin raid].Name}]}) /goto :skip3
	/xtarget set ${b} ${NearestSpawn[${x}, pc paladin raid]}
	/varcalc b ${b}+1
	:skip3
	/next x
	}
:shaman
/if (${Rangers}) {
	/for x 1 to ${Rangers}
		/if (${b} >12) /return
		/if (${Macro.Return.Find[${NearestSpawn[${x}, pc Ranger raid].Name}]}) /goto :skip4
		/xtarget set ${b} ${NearestSpawn[${x}, pc Ranger raid]}
		/varcalc b ${b}+1
		:skip4
	/next x
}
/if (${Me.Class.Name.Equal[shaman]}) {
/if (${Zerkers}) {
/for x 1 to ${Zerkers}
	/if (${b} >12) /return
	/if (${Macro.Return.Find[${NearestSpawn[${x}, pc Berserker raid].Name}]}) /goto :skip5
	/xtarget set ${b} ${NearestSpawn[${x}, pc Berserker raid]}
	/varcalc b ${b}+1
	:skip5
	/next x
	}
/if (${Wizards}) {
/for x 1 to ${Wizards}
	/if (${b} >12) /return
	/if (${Macro.Return.Find[${NearestSpawn[${x}, pc Wizard raid].Name}]}) /goto :skip6
	/xtarget set ${b} ${NearestSpawn[${x}, pc Wizard raid]}
	/varcalc b ${b}+1
	:skip6
	/next x
	}
/if (${Rogues}) {
/for x 1 to ${Rogues}
	/if (${b} >12) /return
	/if (${Macro.Return.Find[${NearestSpawn[${x}, pc Rogue raid].Name}]}) /goto :skip7
	/xtarget set ${b} ${NearestSpawn[${x}, pc Rogue raid]}
	/varcalc b ${b}+1
	:skip7
	/next x
	}
/if (${Necros}) {
/for x 1 to ${Necros}
	/if (${b} >12) /return
	/if (${Macro.Return.Find[${NearestSpawn[${x}, pc Necromancer raid].Name}]}) /goto :skip8
	/xtarget set ${b} ${NearestSpawn[${x}, pc Necromancer raid]}
	/varcalc b ${b}+1
	:skip8
	/next x
	}
/if (${Bards}) {
/for x 1 to ${Bards}
	/if (${b} >12) /return
	/if (${Macro.Return.Find[${NearestSpawn[${x}, pc Bard raid].Name}]}) /goto :skip9
	/xtarget set ${b} ${NearestSpawn[${x}, pc Bard raid]}
	/varcalc b ${b}+1
	:skip9
	/next x
	}
/if (${Monks}) {
/for x 1 to ${Monks}
	/if (${b} >12) /return
	/if (${Macro.Return.Find[${NearestSpawn[${x}, pc Monk raid].Name}]}) /goto :skip9
	/xtarget set ${b} ${NearestSpawn[${x}, pc Monk raid]}
	/varcalc b ${b}+1
	:skip9
	/next x
	}	
}
/return

Sub CheckTanks
/varset MacroState CHECKTANKS
/declare X int local 
/declare i int local 1
|--------Loops Through extended target window For Pets and Players to heal.
/for X 1 to ${Me.XTarget}
/if (${Spawn[${Me.XTarget[${X}].ID}].ID} && (${Spawn[${Me.XTarget[${X}].ID}].Type.Equal[PC]} || ${Spawn[${Me.XTarget[${X}].ID}].Type.Equal[PET]} || ${Spawn[${Me.XTarget[${X}].ID}].Type.Equal[NPC]})) {
	/if (${Spawn[${Me.XTarget[${X}].ID}].PctHPs} <= ${WARPoint} && ${Spawn[${Me.XTarget[${X}].ID}].Distance}<=200 && ${Spawn[${Me.XTarget[${X}].ID}].Type.Equal[PET]} && ${Spawn[${Me.XTarget[${X}].ID}].Level} >99 ) {
		/call Heal ${Spawn[${Me.XTarget[${X}].ID}].PctHPs} ${Spawn[${Me.XTarget[${X}].ID}].ID} WAR
		/return
	} else 	/if (${Spawn[${Me.XTarget[${X}].ID}].Type.NotEqual[Corpse]} && ${Spawn[${Me.XTarget[${X}].ID}].PctHPs} <= ${${Spawn[${Me.XTarget[${X}].ID}].Class.ShortName}Point} && ${Spawn[${Me.XTarget[${X}].ID}].Distance}<=200 && ${Spawn[${Me.XTarget[${X}].ID}].Type.Equal[PC]}) {
		/call Heal ${Spawn[${Me.XTarget[${X}].ID}].PctHPs} ${Spawn[${Me.XTarget[${X}].ID}].ID} ${Spawn[${Me.XTarget[${X}].ID}].Class.ShortName} 
		/return		
	} else 	/if (${Spawn[${Me.XTarget[${X}].ID}].Type.NotEqual[Corpse]} && ${ProtectNPC} && ${ProtectNPCList.Find[${Me.XTarget[${X}].Name}]} && ${Spawn[${Me.XTarget[${X}].ID}].PctHPs} <= 75 && ${Spawn[${Me.XTarget[${X}].ID}].Distance}<=200 && ${Spawn[${Me.XTarget[${X}].ID}].Type.Equal[NPC]}) {
		/call Heal ${Spawn[${Me.XTarget[${X}].ID}].PctHPs} ${Spawn[${Me.XTarget[${X}].ID}].ID} WIZ
		/return
	}
}
/next X
|--------Looks for Tanks in the Raid to add to the extended window.
/if (${Raid.Members} && !${AddTankTimer} && !${ProtectInsideRaid}) {
	/call AddTanks
	/call CreateTimer AddTankTimer
	/varset AddTankTimer 60s
}	
|--------Looks for Players outside the group when not in a raid to add to Xtarget so they can be healed.
/if (!${Raid.Members} && ${ProtectOutsideGroup} && !${ProtectGroupTimer}) {
	/varset i 1
	/varset X 1
	/declare HowMany int local ${Math.Calc[${ProtectOutsidePlayersList.Count[|]}-1]}
	/for X 1 to ${HowMany}
	/if (${Spawn[${ProtectOutsidePlayersList.Arg[${X},|]}].ID} && !${Group.Member[${ProtectOutsidePlayersList.Arg[${X},|]}].ID} && ${i} < 11) {
		/xtarget set ${i} ${ProtectOutsidePlayersList.Arg[${X},|]}
		/varcalc i ${i}+1
	}
	/next X
	/call CreateTimer ProtectGroupTimer 300s
}
|--------Looks for Players Inside the Raid when in a raid to add to Xtarget so they can be healed.
/if (${Raid.Members} && ${ProtectInsideRaid} && !${ProtectInsideRaidTimer}) {
	/varset i 1
	/varset X 1
	/declare HowMany int local ${Math.Calc[${ProtectInsideRaidPlayersList.Count[|]}-1]}
	/for X 1 to ${HowMany}
	/if (${Spawn[${ProtectInsideRaidPlayersList.Arg[${X},|]}].ID} && !${Group.Member[${ProtectInsideRaidPlayersList.Arg[${X},|]}].ID} && ${i} < 11) {
		/xtarget set ${i} ${ProtectInsideRaidPlayersList.Arg[${X},|]}
		/varcalc i ${i}+1
	}
	/next X
	/call CreateTimer ProtectInsideRaidTimer 300s
}
|--------Looks for NPCs to add to Xtarget so they can be healed.
/if (${Raid.Members} && ${ProtectNPC} && !${ProtectNPCTimer}) {
	/varset i 1
	/varset X 1
	/declare HowManyNpc int local ${ProtectNPCList.Count[|]}
	/for X 1 to ${HowManyNpc}
	/if (${Spawn[${ProtectNPCList.Arg[${X},|]}].ID} && ${i} < 11) {
		/tar id ${Spawn[${ProtectNPCList.Arg[${X},|]}].ID}
		/delay 1s ${Target.ID}==${Spawn[${ProtectNPCList.Arg[${X},|]}].ID}
		/xtarget add
		/echo ${ProtectNPCList.Arg[${X},|]}
		/varcalc i ${i}+1
	}
	/next X
	/call CreateTimer ProtectNPCTimer 300s
}
/return ${Me.PctHPs} ${Me.Sitting}

Sub Rod
/declare x int local
/declare AmountOfRods int local ${RodList.Count[|]}
/for x 1 to ${AmountOfRods}
/If (!${FindItem[${RodList.Arg[${x},|]}].Timer} && ${Me.PctMana}<=80 && ${Me.PctHPs}>=54 && ${FindItem[${RodList.Arg[${x},|]}].ID}) {
	/call CastItemClicky "${FindItem[${RodList.Arg[${x},|]}].Name}"
	/return
	}
/next x
/return

Sub CleanUpXTarget
/declare x int local
/for x 1 to 10
/xtarget remove
/next x
/return

Sub DoLeashPerson
/varset MacroState DoLeashPerson
  /declare lsLeashToon string local
  /declare a int local

  /if (${DoLeashToonName.Find[[]}>1) /varset DoLeashToonName ${DoLeashToonName.Right[-${Math.Calc[${DoLeashToonName.Find[[]}-1]}]}

  /for a 1 to ${DoLeashToonName.Count[[]}
    /varset lsLeashToon ${DoLeashToonName.Arg[${a},[].Left[-1]}
    /if (${Spawn[${lsLeashToon}].ID} && ${Spawn[${lsLeashToon}].LineOfSight}) {
      /goto :FollowToon
    }
  /next a

  :FollowToon
   /if (${lsLeashToon.NotEqual[NULL]} && ${Spawn[pc ${lsLeashToon}].ID} && ${Spawn[pc ${lsLeashToon}].Distance}>20 && !${Spawn[pc ${lsLeashToon}].Dead}) {
     /target id ${Spawn[pc ${lsLeashToon}].ID}
     /delay 1s ${Target.CleanName.Equal[${lsLeashToon}]}
     /face fast
	 /melee reset
     /squelch /stick 8
     /delay 2s ${Spawn[pc ${lsLeashToon}].Distance}<10
     /squelch /stick off
   } else /if (!${Spawn[pc ${lsLeashToon}].ID} || ${Spawn[pc ${lsLeashToon}].Dead}) {
     /echo no one on list or everyone is dead!
   }
/return

Sub CastDebuff(MySpell,MySpellType,Int MyTarget,MyTimer,MyTimerLength,MySub)
/doevents
/if (!${MySpell.Find[${Target.Buff[${MySpell}]}]}) {
	/if (${MySub.NotEqual[NULL]}) {
		/call CastSpell "${MySpell}" ${MySpellType} ${MyTarget} ${MySub}
	} else {
		/call CastSpell "${MySpell}" ${MySpellType} ${MyTarget} 
	}
	/if (${castReturn.Equal[CAST_SUCCESS]} || ${castReturn.Equal[CAST_NOHOLD]} || ${castReturn.Equal[CAST_IMMUNE]}) {
		/call CreateTimer ${MyTimer} ${MyTimerLength}
	}  
} else {
	/call CreateTimer ${MyTimer} ${Math.Calc[${Target.BuffDuration[${MySpell}]}*6]}S
}
/return ${castReturn}

Sub RaidBuff
/varset MacroState RaidBuff
/declare i int local
/declare x int local
/declare y int local
/declare RaidMemberID int local
/declare RaidBuffID int local
/declare RaidSpellBase string local
/declare RaidCount int local
/if (${Raid.Members} && ${Me.PctMana} > 10 && ${SpawnCount[raid radius 100]} && !${Me.Casting.ID}) {
	/varset RaidCount ${SpawnCount[raid radius 100]}
	/for i 1 to ${RaidCount}
		/varset RaidMemberID ${Raid.Member[${i}].ID}
		/for x 1 to ${RaidBuffTotal}
		/varset RaidBuffID ${Spell[${RaidBuff${x}}].ID}
		/if (!${RaidBuffTimer${RaidBuffID}${RaidMemberID}} && ${RaidBuffClass${x}.Find[${Spawn[${RaidMemberID}].Class.Name}]} && ${Spawn[${RaidMemberID}].Type.Equal[pc]} && ${Spawn[${RaidMemberID}].Distance} < 100 && ${Me.PctMana}>=10) {
			/if (!${Me.Gem[${RaidBuffCast${x}}]}) /call MemSpell "${RaidBuffCast${x}}" ${BuffGem} 
			/if (!${Me.SpellReady[${RaidBuffCast${x}}]}) /return
			/if (${Stick.Active}) /squelch /stick off
			/target ID ${RaidMemberID}
			/delay 3 ${Target.ID}==${RaidMemberID}
			/delay 3 ${Target.BuffCount}
			/delay 1s
			/if (${RaidBuff${x}.Find[rk. iii]}) {
				/varset RaidSpellBase ${RaidBuff${x}.Left[-7]}
			} else /if (${RaidBuffCast${x}.Find[rk. ii]})  {
				/varset RaidSpellBase ${RaidBuff${x}.Left[-6]}
			} else {
				/varset RaidSpellBase ${RaidBuff${x}}
			}	
			/for y 1 to ${Target.BuffCount}	
			/if (!${Spell[${Target.Buff[${y}]}].WillStack[${Spell[${RaidBuffID}].Name}]} && ${Target.Buff[${y}].ID}) {
				/call CreateTimer RaidBuffTimer${RaidBuffID}${RaidMemberID} ${Spell[${RaidBuffID}].Duration.TotalSeconds}s
				/goto :next
			}
			/next y
			/if (!${Target.Buff[${RaidSpellBase}].ID} && !${Target.Buff[${RaidSpellBase} rk. ii].ID} && !${Target.Buff[${RaidSpellBase} rk. iii].ID} ) {
				/call castSpell "${RaidBuffCast${x}}" ${BuffGem} ${RaidMemberID} 
				/call CreateTimer RaidBuffTimer${RaidBuffID}${RaidMemberID} ${Spell[${RaidBuffID}].Duration.TotalSeconds}s
				/return
			} else {
				/call CreateTimer RaidBuffTimer${RaidBuffID}${RaidMemberID} ${Spell[${RaidBuffID}].Duration.TotalSeconds}s
			} 
		}
		:next
		/next x	
	/next i
}
/call CreateTimer RaidBuffTimer 5s
/return

Sub None

/return

Sub ReturnCurrentGroup
/declare x int local
/declare TempName	string local
/declare GroupList string local
/declare MeleeCount int local 0
/declare CasterCount int local 0
/for x 1 to ${Group}
	/varset TempName |${Group.Member[${x}].Name}[${Group.Member[${x}].Class.ShortName}]
	/varset GroupList ${GroupList}${TempName}
	/if (${Select[${Group.Member[${x}].Class.ShortName},ROG,BER,RNG,MNK,BST]}) /varcalc MeleeCount ${MeleeCount}+1
	/if (${Select[${Group.Member[${x}].Class.ShortName},MAG,WIZ,NEC]}) /varcalc CasterCount ${CasterCount}+1
	/next x
/if (${MeleeCount} > 0 && ${MeleeCount} > ${CasterCount}) {
	/varset GroupList ${GroupList}MeleeGroup
} else /if (${CasterCount} > ${MeleeCount} && ${CasterCount} > 0) {
	/varset GroupList ${GroupList}CasterGroup
} 
/return ${GroupList}

Sub LookForString(StringToCheck,StringToCheckAgainst)
/if (${StringToCheckAgainst.Find[${StringToCheck}]}) /return TRUE
/return FALSE

Sub RaidZone
/if (${Window[Dynamiczonewnd]}) /squelch /windowstate Dynamiczonewnd close
/if (${Zone.Name.Equal[Heart of Fear: The Epicenter]}) {
	/if (${SpawnCount[npc radius 30 "Impalement crystal"]}) {
		/declare tempx float local ${Me.X}
		/declare tempy float local ${Me.Y}
		/moveto loc 158 -222
		/delay 6s !${SpawnCount[npc radius 30 "Impalement crystal"]}
		/moveto stop
		/delay 2s
		/moveto loc ${tempy} ${tempyx}
		/delay 4s
	}
}
/return




