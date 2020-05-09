#event Prepare "[MQ2] prepare"
#event Buff "#1# says, 'Hail, #2#'"
#event HailBuff "[MQ2] hailbuff"

sub clrInit
	/if (!${Defined[MeditateTimer]}) /declare MeditateTimer timer outer
	/if (!${Defined[IsMainTankHurt]}) /declare IsMainTankHurt bool outer
	/if (!${Defined[IsHailBuffEnabled]}) /declare IsHailBuffEnabled bool outer false
	/if (!${Defined[PromiseTimer]}) /declare PromiseTimer timer outer
	/if (!${Defined[AnticipatedTimer]}) /declare AnticipatedTimer timer outer
	/if (!${Defined[HPCheckTimer]}) /declare HPCheckTimer timer outer
	/if (!${Defined[LastHPs]}) /declare LastHPs[6] int outer
	/if (!${Defined[RetortTimer]}) /declare RetortTimer timer outer

/return

sub clrHelp
	/echo -- ${Me.Class.Name} Commands --
	/echo is MQ2Cleric enabled? (${IsPluginEnabled[mq2cleric]})
	/echo /echo hailbuff: is cleric is hailed, buff a player? (${IsHailBuffEnabled})
	/echo /echo prepare: prepare main tank for an incoming harder pull
/return

Sub clr
:Main
	/if (${Me.Ducking}) {
		/delay 1s
		/doevents
		/goto :Main
	}
	/if (${Me.Dead}) /endmacro
	/call Engage
	/call Nuke
	/call Heal	
	/call CombatBuffs
	/call IdleBuffs
	/call Meditate
	/call DivineGuardian
	/delay 1s
	/doevents
	/goto :Main
/return

sub Event_HailBuff
	/if (!${Defined[IsHailBuffEnabled]}) /declare IsHailBuffEnabled bool outer false
	/varcalc IsHailBuffEnabled (!${IsHailBuffEnabled})
	/echo Is buff on hail enabled? ${IsHailBuffEnabled}
/return

Sub Event_Prepare
	/if (!${Group.MainTank.ID}) {
		/echo no main tank to prepare!
		/return
	}
	/echo doing a preparation of a pull	
	/varset SpellName Hand of Sincere Infusion
	/if (${Me.Gem[${SpellName}]}) {
		/casting "${SpellName}" -targetid|${Group.MainTank.ID}
		/delay 5s
		/delay 1s ${Cast.Status.NotEqual[C]}
		/doevents
	}

	/varset SpellName Promised Redemption Rk. II
	/if (${Me.Gem[${SpellName}]}) {
		/casting "${SpellName}" -targetid|${Group.MainTank.ID}
		/delay 5s
		/delay 1s ${Cast.Status.NotEqual[C]}
		/doevents
	}

	/casting "Curate's Retort" gem13 -targetid|${Group.MainTank.ID}	
	/delay 5s
	/delay 1s ${Cast.Status.NotEqual[C]}
	/doevents
	/casting "Anticipated Imposition" gem13 -targetid|${Group.MainTank.ID}	
	/delay 5s
	/delay 1s ${Cast.Status.NotEqual[C]}
	/doevents
	/casting "Divine Intermediation Rk. II" gem13 -targetid|${Group.MainTank.ID}	
	/delay 5s
	/delay 1s ${Cast.Status.NotEqual[C]}
	/doevents

	/varset SpellName Issuance of Sincerity
	/casting "${SpellName}" -targetid|${Group.MainTank.ID}
	/delay 2s
	/delay 1s ${Cast.Status.NotEqual[C]}
	/doevents

	/echo ok, should be ready
/return


Sub Event_Buff(line, playerTarget, hailTarget)
	/if (!${IsHailBuffEnabled}) /return
	/if (!${Defined[hailTarget]}) /declare hailTarget string ${Me.Name}
	/if (!${Defined[playerTarget]}) /declare playerTarget string ${Target.Name}
	/if (${hailTarget.NotEqual[${Me.Name}]}) /return
	/echo buffing ${playerTarget}
	
	/if (!${Defined[BuffTimer]}) /declare BuffTimer timer outer	
	/if (${BuffTimer}>0) {
		/echo ignoring ${playerTarget}
		/return
	}
	/varset BuffTimer 15s
	/target pc ${playerTarget}
	/delay 5
	/doevents
	
	/call CastMem "Unified Hand of Righteousness"	
	/call CastMem "Rallied Greater Protection of Vie"
	/call CastMem "Hand of Will"
	/call CastMem "Unified Hand of Jorlleag"
	/varset SpellName Shining Aegis
	
/return


Sub Nuke
	/if (${Window[CastingWindow]}) /return
	/if (!${Target.ID}) /return
	/if (${Me.Invis}) /return
	/if (${Target.Distance}>100) /return
	/if (${Target.PctHPs}>99) /return
	/if (!${Target.Type.Equal[NPC]}) /return
	/if (!${Target.LineOfSight}) /return
	/if (!${IsMainTankHurt} && ${Group.MainTank.PctHPs}<50) {
		/varset IsMainTankHurt TRUE
	}

	/if (!${IsXTarget[${Target.ID}]}) /return	

	/if (${Me.XTarget} > 1) {
		|TODO: Repel Wicked
	}
	| Turn Undead
	/varset SpellName Turn Undead
	/if (${Target.ID} && ${Target.Body.Name.Equal[Undead]} && ${Target.LineOfSight} && ${Cast.Ready[${SpellName}]} && ${Target.PctHPs} > 50 && ${Target.Level} >= 110) {
		/casting "${SpellName}" -maxtries|3
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana}mana
		/return
	}

	/varset SpellName Obliterate the Undead Rk. II
	/if (${Target.ID} && ${Target.Body.Name.Equal[Undead]} && ${Me.PctMana}>70 && ${Target.LineOfSight} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}" -maxtries|3
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana}mana
		/return
	}

	| Stun DD
	/varset SpellName Sound of Rebuke
	/if (${Target.ID} && ${Target.LineOfSight} && ${Cast.Ready[${SpellName}]} && ${Me.PctMana}>80 && ${Target.Level} <= 115) {
		/casting "${SpellName}" -maxtries|3
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana}mana
		/return
	}


	/varset SpellName Mending Splash Rk. II
	/if (!${Defined[MendingTimer]}) /declare MendingTimer timer outer
	/if (!${MendingTimer} && ${Cast.Ready[${SpellName}]}) {
		/varset MendingTimer 60s 
		/casting "${SpellName}" -targetid|${Me.ID} -maxtries|3
		/echo ${SpellName} ${Group.MainTank.Name} ${Spell[${SpellName}].Mana}mana
	}

	
	/varset SpellName Mark of Wenglawks
	/if (!${Defined[LastDSID]}) /declare LastDSID int outer
	/if (${Me.XTarget}== 1 && ${Target.PctHPs}>50 && ${Target.Level} > 107 && ${Cast.Ready[${SpellName}]} && !${Target.Buff[${SpellName}].ID} && !${LastDSID} != ${Target.ID}) {
		/casting "${SpellName}" -maxtries|3
		/varset LastDSID ${Target.ID}
		/return
	}

	
	/varset SpellName Virtuous Contravention
	/if ((${Target.PctHPs}>5 || ${Target.Named}) && ${Me.PctMana}>80 && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana}mana
		/return
	}

	| 10m aat40
	/varset SpellName Fundament: Third Spire of Divinity
	/if (${SpawnCount[id ${BurnID}]} && ${Cast.Ready[${SpellName}]}) {
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana}mana
		/casting "${SpellName}"
		/return
	}


	| 20m aa t11 exquisite benediction healing ward (burn)
	/varset SpellName Exquisite Benediction
	/if (${SpawnCount[id ${BurnID}]} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana} burnmana
	}

	| 27m aa t13 divine retribution (stun defensive) (burn)
	/varset SpellName Divine Retribution
	/if (${SpawnCount[id ${BurnID}]} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana} burnmana
	}


	| 7.5m aa t3 celestial regeneration (burn)
	/varset SpellName Celestial Regeneration
	/if (${SpawnCount[id ${BurnID}]} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana} burnmana
	}

	| 5m aat61 focused celestial regeneration 18s instant cast
	|/varset SpellName Focused Celestial Regeneration
	|/if (${SpawnCount[id ${BurnID}]} && ${Cast.Ready[${SpellName}]} && ${Group.MainTank.Distance}<100) {
		|/casting "${SpellName}" -targetid|${Group.MainTank.ID}
		|/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana} burnmana
	|}

	| 10m aa t12 celestial hammer attack mob (burn)
	/varset SpellName Celestial Hammer
	/if (${SpawnCount[id ${BurnID}]} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana} burnmana
	}

	| 10m aa t73 celestial rapidity reduce cast time 50% 36s (burn)
	/varset SpellName Celestial Rapidity
	/if (${SpawnCount[id ${BurnID}]}&& ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana} burnmana
	}

	| 5m aa t30 shield of reverence (70% reflect for 18s) burn
	/varset SpellName Shield of Reverence
	/if (${SpawnCount[id ${BurnID}]} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}" 
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana} burnmana
	}
	
	/varset SpellName Ward of Assurance
	/if (${SpawnCount[id ${BurnID}]} && ${Cast.Ready[${SpellName}]} && ${Group.MainTank.Distance}<100) {
		/casting "${SpellName}" -targetid|${Group.MainTank.ID}
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana} burnmana
	}

	/varset SpellName Issuance of Sincerity
	/if ((${SpawnCount[id ${BurnID}]} || ${Me.XTarget}>2) && ${Cast.Ready[${SpellName}]} && ${Group.MainTank.Distance}<100) {
		/casting "${SpellName}" -targetid|${Group.MainTank.ID}
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana} burnmana
	}

	/call CureCheck

	/if (${Target.PctHPs}>99) /return


	/varset SpellName Merciful Contravention
	/if (${Cast.Ready[${SpellName}]} && ${Me.PctMana}>80 && ${Target.Level}<=110) {
		/casting "${SpellName}" -maxtries|3
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana}mana
		/return
	}


	
	/varset SpellName Unyielding Judgment
	/if (${Cast.Ready[${SpellName}]} && ${Me.Song[Healing Twincast].ID} == 0 && ${Me.PctMana}>90) {
		/casting "${SpellName}" -maxtries|3
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana}mana
	}

	/varset SpellName Obliterate the Undead Rk. II
	/if (${Target.Body.Name.Equal[Undead]} && ${Cast.Ready[${SpellName}]} && ${Me.PctMana}>90) {
		/casting "${SpellName}" -maxtries|3
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana}mana
	}


	/varset SpellName Chromaburst Rk. II
	/if (${Cast.Ready[${SpellName}]} && ${Me.PctMana}>90) {
		/casting "${SpellName}" -maxtries|3
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana}mana
	}

	/if (${Me.Pet.ID}>0 && ${Me.Pet.Target.ID}==0 && ${Target.PctHPs}<95) {
		/pet attack
	} 

	
	| Hammer
	/if (!${Defined[HammerTimer]}) /declare HammerTimer timer outer
	/varset SpellName Unyielding Hammer of Obliteration
	/if (${Cast.Ready[${SpellName}]} && ${Me.PctMana}>80 && !${Me.Pet.ID}) {
		/casting "${SpellName}"  -maxtries|3
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana}mana
		/return
	}
/return

Sub CombatBuffs
	/if (${Me.Moving}) /return
	/if (!${IsInCombat[]}) /return
	/if (${Window[CastingWindow]}) /return
	/if (${Zone.ShortName.Equal[guildhallsml_int]}) /return
	/call PromiseTank
	/if (${Macro.Return}) /return
	/declare i int local
	/varset SpellName Miniature Horn of Unity
	/if (!${Me.Invis} && ${Cast.Ready[${SpellName}]} && ${Me.Song[Blessing of Unity].ID} == 0 && ${Me.PctMana}<80) {
		/casting "${SpellName}"
		/return
	}

	/varset SpellName Veturika's Perseverance
	/if (!${Me.Buff[Resurrection Sickness].ID} && ${Me.PctMana}<80 && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo self mana
		/return
	}

	/if (${Me.PctMana}<50) /return
	

	/if (!${Defined[VieTimer]}) /declare VieTimer timer outer
	/varset SpellName Rallied Greater Guard of Vie Rk. II
	/if (!${VieTimer} && ${Cast.Ready[${SpellName}]} && ${Group.MainTank.ID}>0 && ${Spawn[id ${Group.MainTank.ID}].Distance}<100 && ${Spawn[id ${Group.MainTank.ID}].LineOfSight}) {		
		/casting "${SpellName}" gem13 -targetid|${Group.MainTank.ID}
		/echo ${SpellName} -> ${Group.MainTank.Name}  ${Spell[${SpellName}].Mana}mana
		/varset VieTimer 180s
	}

	/varset SpellName Curate's Retort
	/if (${RetortTimer} == 0 && ${Cast.Ready[${SpellName}]} && ${IsMainTankHurt} && ${Group.MainTank.ID}>0 && ${Spawn[id ${Group.MainTank.ID}].Distance}<100 && ${Spawn[id ${Group.MainTank.ID}].LineOfSight}) {
		/casting "${SpellName}" -targetid|${Group.MainTank.ID}
		/echo ${SpellName} -> ${Group.MainTank.Name} ${Spell[${SpellName}].Mana}mana
		/varset RetortTimer 60s
	}


	
	/if (!${Defined[YaulpTimer]}) /declare YaulpTimer timer outer
	/varset SpellName Yaulp XVI
	/if (!${Me.Buff[${SpellName}].ID} && !${YaulpTimer}) {
		/casting Yaulp
		/echo ${SpellName} ${Spell[${SpellName}].Mana}mana
		/varset YaulpTimer 60s
		/return
	}

	/varset SpellName Hand of Sincere Infusion
	/if (${Me.Song[${SpellName}].ID} == 0 && ${Cast.Ready[${SpellName}]} && ${Me.PctMana} > 80 && ${IsMainTankHurt}) {
		/casting "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana}mana
		/return
	}


	/declare isNear bool inner		
	/for i 2 to ${Me.XTarget}
		/if (${Me.XTarget[${i}].Distance} < 100 && ${Me.XTarget[${i}].LineOfSight}) {
			/varset isNear TRUE
		}
	/next i
	/if (!${isNear}) /return

	|/varset SpellName Talisman of the Leopard Rk. II
	|/if (${Me.Buff[${SpellName}].ID} == 0 && ${Me.Buff[Roar of the Lion 1].ID}==0 && ${Me.Buff[Dissident Roar 1].ID} == 0 && ${Target.PctHPs}>20) {
	|	/casting "${SpellName}" -maxtries|3 -invis
	|	/return
	|}

	|/if (${Me.Buff[Specter of Renewal].ID} == 0) {
	|	/casting "Specter of Renewal" gem7 -maxtries|3 -invis
	|	/return
	|}
	/if (!${Me.Invis} && ${Cast.Ready[${SpellName}]} && ${Me.Song[Blessing of Unity].ID} == 0 && ${Me.PctEndurance}<80) {
		/casting "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Spell[${SpellName}].Mana}mana
		/return
	}
/return

Sub CureCast(gID)
	/varset SpellName Radiant Cure
	/if (${Cast.Ready[${SpellName}]} && ${Spawn[id ${gID}].Distance}<100) {
		/casting "${SpellName}"
		/return true
	}

	/varset SpellName Purify Soul
	/if (${Cast.Ready[${SpellName}]} && ${Spawn[id ${gID}].Distance}<200) {
		/casting "${SpellName}" -targetid|${gID}
		/return true
	}
/return false

Sub PromiseTank
	/if (${Group.MainTank.Distance}>200) /return false
	/if (!${Group.MainTank.Present}) /return false
	/if (!${Defined[dout]}) /declare dout string local
	/varset SpellName Promised Redemption Rk. II
	/if (!${Cast.Ready[${SpellName}]}) /return false
	/if (!${Me.Gem[${SpellName}]}) /return false
	/if (!${Defined[IsBurnQueryEnabled]}) /declare IsBurnQueryEnabled bool outer

	/if (${Group.MainTank.Distance}>100) /return false
	/if (!${BurnID} && ${Me.XTarget} < 1 && !${IsMainTankHurt}) /return false
	/if (${Group.MainTank.Moving}) /return false

	| Timer based if I can't dquery
	/if (!${IsBurnQueryEnabled}) {
		/if (!${PromiseTimer}) /return false
		/if (${Me.XTarget[1].PctHPs} < 50) /return false
		/casting "${SpellName}" -targetid|${Group.MainTank.ID} -maxtries|3
		/echo ${SpellName} ${Group.MainTank.Name} ${Spell[${SpellName}].Mana}mana timer
		/varset PromiseTimer 21s
		/return true
	}

	| dquery check for promise
	/dquery ${Group.MainTank.Name} -q "Me.Buff[${SpellName}].ID" -o dout
	/delay ${DanNetDelay} ${DanNet.Query.Received}
	/if (${Int[${dout}]}) /return false
	/casting "${SpellName}" -targetid|${Group.MainTank.ID} -maxtries|3
	/echo ${SpellName} ${Group.MainTank.Name} ${Spell[${SpellName}].Mana}mana dquery
/return true

Sub CureCheck
	/if (!${Defined[CureTimer]}) /declare CureTimer timer outer
	/if (${CureTimer}) /return
	/if (!${IsLongBuffOK[]}) /return
	
	/if (!${Defined[dout]}) /declare dout string local
	/if (!${Defined[gi]}) /declare gi int local
	/if (!${Defined[gName]}) /declare gName string local
	/if (!${Defined[gID]}) /declare gID int local

	/varset CureTimer 10s
	/for gi 0 to ${Group.Members}		
		/if (!${Group.Member[${gi}].Present}) /continue
		/if (${Group.Member[${gi}].Mercenary}) /continue
		/if (${Group.Member[${gi}].Offline}) /continue
		/if (${Group.Member[${gi}].OtherZone}) /continue	
		/if (${Group.Member[${gi}].Distance}>200) /continue
		/if (${Group.Member[${gi}].Type.Equal[Corpse]}) /continue
		/varset gName ${Group.Member[${gi}].Name}
		/varset gID ${Group.Member[${gi}].ID}

		/dquery ${gName} -q "Me.Poisoned.ID" -o dout
		/delay ${DanNetDelay} ${DanNet.Query.Received}
		/if (${gName.Equal[${Me.Name}]}) /varset dout ${Me.Poisoned.ID}
		/if (${Int[${dout}]}) {			
		/call CureCast ${gID}
			/if (${Macro.Return}) /dgtell curing ${gName} of Poison
			/return
		}

		/dquery ${gName} -q "Me.Diseased.ID" -o dout
		/delay ${DanNetDelay} ${DanNet.Query.Received}
		/if (${gName.Equal[${Me.Name}]}) /varset dout ${Me.Diseased.ID}
		/if (${Int[${dout}]}) {			
			/call CureCast ${gID}
			/if (${Macro.Return}) /dgtell curing ${gName} of Poison
			/return
		}
		
		/dquery ${gName} -q "Me.Cursed.ID" -o dout
		/delay ${DanNetDelay} ${DanNet.Query.Received}
		/if (${gName.Equal[${Me.Name}]}) /varset dout ${Me.Cursed.ID}
		/if (${Int[${dout}]}) {			
			/call CureCast ${gID}
			/if (${Macro.Return}) /dgtell curing ${gName} of Poison
			/return
		}

		/dquery ${gName} -q "Me.Corrupted.ID" -o dout
		/delay ${DanNetDelay} ${DanNet.Query.Received}
		/if (${gName.Equal[${Me.Name}]}) /varset dout ${Me.Corrupted.ID}
		/if (${Int[${dout}]}) {			
			/call CureCast ${gID}
			/if (${Macro.Return}) /dgtell curing ${gName} of Poison
			/return
		}

		/dquery ${gName} -q "Me.Rooted.ID" -o dout
		/delay ${DanNetDelay} ${DanNet.Query.Received}
		/if (${gName.Equal[${Me.Name}]}) /varset dout ${Me.Rooted.ID}

		/if (${Int[${dout}]}) {			
			/call CureCast ${gID}
			/if (${Macro.Return}) /dgtell curing ${gName} of Root 
			/return
		}

		/dquery ${gName} -q "Me.Snared.ID" -o dout
		/delay ${DanNetDelay} ${DanNet.Query.Received}
		/if (${gName.Equal[${Me.Name}]}) /varset dout ${Me.Snared.ID}

		/if (${Int[${dout}]}) {			
			/call CureCast ${gID}
			/if (${Macro.Return}) /dgtell curing ${gName} of Snared 
			/return
		}

		/dquery ${gName} -q "Me.Maloed.ID" -o dout
		/delay ${DanNetDelay} ${DanNet.Query.Received}
		/if (${gName.Equal[${Me.Name}]}) /varset dout ${Me.Maloed.ID}

		/if (${Int[${dout}]}) {
			/call CureCast ${gID}
			/if (${Macro.Return}) /dgtell curing ${gName} of Maloe
			/return
		}

		/dquery ${gName} -q "Me.Tashed.ID" -o dout
		/delay ${DanNetDelay} ${DanNet.Query.Received}
		/if (${gName.Equal[${Me.Name}]}) /varset dout ${Me.Tashed.ID}

		/if (${Int[${dout}]}) {			
			/call CureCast ${gID}
			/if (${Macro.Return}) /dgtell curing ${gName} of Tashed 
			/return
		}
	/next gi

/return

Sub IdleBuffs
	/if (${Me.Moving}) /return
	/if (${IsInCombat[]}) /return
	/if (${Window[CastingWindow]}) /return
	/if (${Me.PctMana}<50) /return
	/if (${BuffCheckTimer}>0) /return
	/if (${Me.Invis}) /return
	/if (${Zone.ShortName.Equal[guildhallsml_int]}) /return
	/if (!${Defined[CureTimer]}) /declare CureTimer timer outer	
	/if (${Me.SpellInCooldown}) /return
	
	/if (${IsMainTankHurt}) {
		|/call PromiseTank
		/if (${Macro.Return}) /return
	}

	/if (${Me.CombatState.Equal[COOLDOWN]}) {
		
		/if (!${Defined[YaulpTimer]}) /declare YaulpTimer timer outer
		/varset SpellName Yaulp XV Rk. II
		/if (${Me.Buff[${SpellName}].ID} == 0 && ${Cast.Ready[${SpellName}]} && !${YaulpTimer}) {
			/casting "${SpellName}"
			/echo ${SpellName} ${Spell[${SpellName}].Mana}mana
			/varset YaulpTimer 60s
			/return
		}
	}
	/call CureCheck

	/varset SpellName Anticipated Intervention
	/if (${Me.Gem[${SpellName}]} && ${Group.MainTank.Distance}<=100 && !${AnticipatedTimer} && ${Me.PctMana}>80 && ${IsMainTankHurt}) {
		/casting "${SpellName}" -targetid|${Group.MainTank.ID} gem13 -maxtries|3
		/echo ${SpellName} ${Group.MainTank.Name} ${Spell[${SpellName}].Mana}mana
		/varset AnticipatedTimer 300s
	}
	

	/varset SpellName Miniature Horn of Unity
	/if (!${Me.Invis} && ${Cast.Ready[${SpellName}]} && ${Me.Song[Blessing of Unity].ID} == 0) {
		/casting "${SpellName}"
		/return
	}

	/varset SpellName Divine Reaction Rk. II
	/if (${Me.Buff[${SpellName}].ID} == 0) {
		/casting "${SpellName}" gem13 -maxtries|3 -invis
		/return
	}

	/varset SpellName Aura of Divinity
	/if (${Me.Song[${SpellName}].ID}==0) {
		/removeaura Aura of Divinity
		/casting "${SpellName}" gem13
		/return
	}

	/varset SpellName Aura of the Pious
	/if (${Me.Song[${SpellName}].ID}==0) {
		/removeaura Aura of the Pious
		/casting "${SpellName}" gem13
		/return
	}

	/call GroupBuff "Unified Hand of Righteousness" "Righteousness"
	|/call GroupBuff "Unified Hand of Jorlleag" "Ward of Sincerity"

	|/call GroupBuff "Rallied Greater Protection of Vie" "Rallied Greater Protection of Vie"
	|/call GroupBuff "Hand of Will" "Hand of Will"

	/call DI


	| Spell Focus
	|/varset SpellName Unified Hand of Assurance Rk. II
	|/if (${Me.Buff[Assurance Rk. II].ID} == 0 && ${Spell[Benediction of Sanctity Rk. II].Stacks} && ${Spell[Assurance Rk. II].Stacks}) {
	|	/casting "${SpellName}" gem13 -maxtries|3 -invis
	|	/return
	|}


	| Armor
	/varset SpellName Armor of Sincerity
	/if (${Me.Buff[${SpellName}].ID} == 0 && ${Spell[${SpellName}].Stacks}) {
		/casting "${SpellName}" gem13 -maxtries|3 -invis
		/return
	}


/return

Sub HealSnapshot
	/if (${HPCheckTimer}>0) /return
	/varset HPCheckTimer 1s
	/declare i int local
	/for i 0 to ${Group.Members}
		/varset LastHPs[${Math.Calc[${i}+1]}] ${Group.Member[${i}].PctHPs}
	/next i
/return

Sub LastHPDifference(playerName)
	/declare i int local
	/for i 0 to ${Group.Members}
		/if (${Group.Member[${i}].Name.Equal[${playerName}]}) /return ${Math.Calc[${LastHPs[${i}]}-${Group.Member[${i}].PctHPs}]}
	/next i
/return 0


| use epic when player is >200m
sub EpicHeal
	/varset SpellName Harmony of the Soul
	/if (${Navigation.Active}) /return	
	/if (${Window[CastingWindow]}) /return	
	/if (${Me.Moving}) /return	
	/if (!${Cast.Ready[${SpellName}]}) /return
	/if (${Me.SpellInCooldown}) /return
	/if (${Me.Invis} && ${Group} && ${Group.MainTank.Name.Length}>0) {
		/dquery ${Group.MainTank} -q "Me.Invis" -o dout
		/delay ${DanNetDelay} ${DanNet.Query.Received}

		/if (!${Bool[${dout}]}) {
			/return
		}
	}
	
	/for i 0 to ${Group.Members}
		/if (!${Group.Member[${i}].Present}) /continue
		/if (${Group.Member[${i}].Mercenary}) /continue
		/if (${Group.Member[${i}].Offline}) /continue
		/if (${Group.Member[${i}].OtherZone}) /continue
		/if (${Group.Member[${i}].Distance} >= 200) /continue
		/if (${Group.Member[${i}].Distance} < 1000) /continue
		/if (${Group.Member[${i}].Type.Equal[Corpse]}) /continue
		/if (${Group.Member[${i}].PctHPs} > 60) /continue
		
		/dgtell epic click on ${Group.Member[${i}].Name} from ${${Group.Member[${i}].Distance}m
		/casting ${SpellName} -targetid|${Group.Member[${i}].Name}
		/return
	/next i
/return

Sub Heal
	/if (${IsPluginEnabled[mq2cleric]}) /return

	/if (${IsMainTankHurt} && ${Me.XTarget}==0) /varset IsMainTankHurt false
	/if (!${Defined[dout]}) /declare dout string local

	/if (${Navigation.Active}) /return	
	
	/if (${Window[CastingWindow]}) /return	
	/if (${Me.Moving}) /return	
	
	/if (${Me.SpellInCooldown}) /return
	
	/if (${Me.Invis} && ${Group} && ${Group.MainTank.Name.Length}>0) {
		/dquery ${Group.MainTank} -q "Me.Invis" -o dout
		/delay ${DanNetDelay} ${DanNet.Query.Received}

		/if (!${Bool[${dout}]}) {
			/return
		}
	}
	/declare healCount int local
	/call HealSnapshot

	/declare highHP int local 80
	/declare veryLowHP int local 45
	/declare i int local
	
	/declare healTargetPctHPs int local
	/declare healTargetID int local
	/declare healTargetName string local
	/declare lastCastTargetID int local
	/declare hpDifference int local
	
	
	/varset healTargetPctHPs 100
	/varset healCount 0

	/for i 0 to ${Group.Members}		
		/if (${healTargetPctHPs} < ${Group.Member[${i}].PctHPs}) /continue
		/if (!${Group.Member[${i}].Present}) /continue
		/if (${Group.Member[${i}].Mercenary}) /continue
		|/if (${Group.Member[${i}].Offline}) /continue
		/if (${Group.Member[${i}].OtherZone}) /continue
		/if (${Group.Member[${i}].Distance}>200) /continue
		/if (${Group.Member[${i}].Type.Equal[Corpse]}) /continue
		/if (${Group.Member[${i}].PctHPs} < ${highHP}) /varcalc healCount ${healCount}+1

		/if (${healTargetPctHPs} < ${Group.Member[${i}].PctHPs}) /continue

		/varset healTargetName ${Group.Member[${i}].Name}
		/varset healTargetPctHPs ${Group.Member[${i}].PctHPs}
		/varset healTargetID ${Group.Member[${i}].ID}

		|/varset SpellName Divine Peace
		|/if (${Cast.Ready[${SpellName}]} && ${healTargetID} == ${Me.ID} && ${healTargetPctHPs}<70) {
		|	/casting "${SpellName}"
		|	/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
		|	/return
		|}

		/varset SpellName Sanctuary
		/if (${Cast.Ready[${SpellName}]} && ${healTargetID} == ${Me.ID} && ${healTargetPctHPs}<50) {
			/casting "${SpellName}"	
			/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
			/return
		}
	/next i


	/for i 2 to 13
		/if (!${Me.XTarget[${i}].ID}) /continue
		/if (${Me.XTarget[${i}].TargetType.Equal[Auto Hater]}) /continue
		/if (${Me.XTarget[${i}].TargetType.Equal[Group Assist Target]}) /continue
		/if (${Me.XTarget[${i}].TargetType.Equal[Raid Assist 1 Target]}) /continue
		/if (${Me.XTarget[${i}].Type.Equal[NPC]} && !${Spawn[group ${Me.XTarget[${i}].Master}].ID}) /continue
		|/if (${Me.XTarget[${i}].Type.NotEqual[PC]} && ${Me.XTarget[${i}].Type.NotEqual[NPC]}) /continue
		/if (${healTargetPctHPs} < ${Me.XTarget[${i}].PctHPs}) /continue

		/if (${Me.XTarget[${i}].Distance}>200) /continue
		
		/varset healTargetName ${Me.XTarget[${i}].Name}
		/varset healTargetPctHPs ${Me.XTarget[${i}].PctHPs}
		/varset healTargetID ${Me.XTarget[${i}].ID}
		/varset healCount 1
	/next i

	/if (!${healTargetID}) /return
	/call LastHPDifference "${healTargetName}"
	/varset hpDifference ${Macro.Return}

	/if (${healTargetPctHPs} > ${highHP}) /return
		
	/dgtell I need to  heal ${healTargetName} ${hpDifference} they're at ${healTargetPctHPs}

	/if (${healTargetPctHPs} <= ${veryLowHP} || ${hpDifference}>40) {
		/call GenericPriorityHeal ${hpDifference} ${healTargetID}
		/if (${Macro.Return}) /return
	}


	/varset SpellName Dissident Blessing
	/if (${Cast.Ready[${SpellName}]} && ${Spawn[id ${healTargetID}].PctHPs}<50) {
		/casting "${SpellName}" -targetid|${healTargetID} -maxtries|3		
		/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
		/return
	}
	
	/if (${healCount}>1) {
		/echo Hmm multiple people need healing...
		
		/varset SpellName Word of Greater Replenishment Rk. II
		/if (${Cast.Ready[${SpellName}]}) {
			/casting "${SpellName}" -targetid|${healTargetID} -maxtries|3
			/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
			/return
		}

		/varset SpellName Divine Arbitration
		/if (${hpDifference}>40 && ${Cast.Ready[${SpellName}]}) {
			/casting "${SpellName}"
			/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
			/return
		}

		/varset SpellName Celestial Regeneration
		/if (${hpDifference}>40 && ${Cast.Ready[${SpellName}]}) {
			/casting "${SpellName}"
			/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
			/return
		}
	}


	/varset SpellName Sincere Remedy Rk. II
	/if (${Cast.Ready[${SpellName}]} && ${healTargetPctHPs}>70) {
		/casting "${SpellName}" -targetid|${healTargetID} -maxtries|3		
		/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
		/return
	}
	
	
	/varset SpellName Merciful Remedy Rk. II
	/if (${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}" -targetid|${healTargetID} -maxtries|3				
		/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
		/return
	}


	/varset SpellName Spiritual Remedy Rk. II
	/if (${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}" -targetid|${healTargetID} -maxtries|3		
		/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
		/return
	}	

	/varset SpellName Graceful Remedy
	/if (${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}" -targetid|${healTargetID} -maxtries|3		
		/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
		/return
	}	


	/varset SpellName Dissident Blessing
	/if (${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}" -targetid|${healTargetID} -maxtries|3		
		/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
		/return
	}

	/varset SpellName Sincere Intervention
	/if (${Cast.Ready[${SpellName}]} && ${healTargetPctHPs}>70) {
		/casting "${SpellName}" -targetid|${healTargetID} -maxtries|3		
		/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
		/return
	}


	/echo falling back to priority heals, all normal ones on cooldown
	/call GenericPriorityHeal ${hpDifference} ${healTargetID}
	/if (${Macro.Return}) /return	
/return


Sub GenericPriorityHeal(int hpDifference, int healTargetID)

	/varset SpellName Sanctuary
	/if (${Cast.Ready[${SpellName}]} && ${healTargetID} == ${Me.ID} && ${healTargetPctHPs}<50) {
		/casting "${SpellName}" -targetid|${healTargetID} -maxtries|3		
		/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
		/return
	}

	/varset SpellName Divine Arbitration
	/if (${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}" -targetid|${healTargetID}
		/varset healTargetID ${healTargetID}
		/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
		/return true
	}

	/varset SpellName Beacon of Life
	/if (${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}" -targetid|${healTargetID} -maxtries|3
		/varset healTargetID ${healTargetID}
		/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
		/return true
	}	
	
	/varset SpellName Celestial Regeneration
	/if (${hpDifference}>40 && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
		/return true
	}

	/varset SpellName Harmony of the Soul
	/if (${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} ->${Spawn[id ${healTargetID}].Name} (${Spawn[id ${healTargetID}].PctHPs}%) (${hpDifference}%diff)
		/return true
	}
/return false

Sub Meditate
/if (${Stick.Status.Equal[ON]}) /return
	/if (${Navigation.Active}) /return
	/if (!${Me.CombatState.Equal[ACTIVE]} && !${Me.CombatState.Equal[RESTING]}) /return
	/if (${Me.XTarget}>1) /return
	/if (${Window[CastingWindow]}) /return	
	/if (${Me.SpellInCooldown}) /return
	/if (${MeditateTimer}>0) /return
	/if (${Me.Moving}) {
		/echo interrupted meditate
		/varset MeditateTimer 60s
		/return
	}
	/if (${Me.XTarget}<2) /return	
	/if (${Zone.Outdoor}) {
		/if (${Me.Buff[Lava Braxi].ID}==0) /casting "Lava Braxi Saddle" -invis
		/return
	}
	/if (${Me.PctMana}<90 && !${Me.Sitting}) /sit
/return


Sub DivineGuardian
	/if (${Me.Moving}) /return
	/if (${Me.SpellInCooldown}) /return
	/if (${Zone.ShortName.Equal[guildhallsml_int]}) /return
	/if (${Me.Invis}) /return

	| 7m aat75
	/if (!${Defined[GuardianTimer]}) /declare GuardianTimer timer outer
	/varset SpellName Divine Guardian
	/if (!${GuardianTimer} && ${Cast.Ready[${SpellName}]} && ${Group.MainTank.ID}>0 && ${Spawn[id ${Group.MainTank.ID}].Distance}<100 && ${Spawn[id ${Group.MainTank.ID}].LineOfSight}) {
		/varset GuardianTimer 180s
		/casting "${SpellName}" -targetid|${Group.MainTank.ID}
		/echo ${SpellName} -> ${Group.MainTank.Name}  ${Spell[${SpellName}].Mana}mana
	}
/return

sub DI
	/if (!${FindItemCount[Emerald]}) /return
	
	/varset SpellName Divine Intermediation Rk. II
	/if (!${Defined[dout]}) /declare dout string local
	/if (!${Group.MainTank.ID}) /return
	/if (!${Cast.Ready[${SpellName}]}) /return
	/dquery ${Group.MainTank.Name} -q "Me.Buff[${SpellName}].ID" -o dout

	/delay ${DanNetDelay} ${DanNet.Query.Received}

	/if (${Int[${dout}]}) /return

	/if (${Spawn[id ${Group.MainTank.ID}].Distance}>200) /return	
	/if (!${Spawn[id ${Group.MainTank.ID}].LineOfSight}) /return
	/casting "${SpellName}" -targetid|${Group.MainTank.ID}
	/echo ${SpellName} -> ${Group.MainTank.Name}  ${Spell[${SpellName}].Mana}mana
/return