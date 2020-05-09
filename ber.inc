#event Alliance "[MQ2] alliance"
#event Grind "[MQ2] grind"
#event AE "[MQ2] ae"
#event Decapatation "#1# hit #2# for #3# points of physical damage by Decpitation#4#"
#event Decapatation "#*#Decpitation#4#"

sub berInit
	/if (!${Defined[IsAllianceEnabled]}) /declare IsAllianceEnabled bool outer FALSE	
	/if (!${Defined[IsGrindEnabled]}) /declare IsGrindEnabled bool outer FALSE
	/if (!${Defined[IsAEEnabled]}) /declare IsAEEnabled bool outer FALSE
	/if (!${Defined[GrindBurnID]}) /declare GrindBurnID int outer
	/if (!${Defined[ComponentNagTimer]}) /declare ComponentNagTimer timer outer
/return

sub berHelp
	/echo -- ${Me.Class.Name} Commands --
	/echo is MQ2Berzerker enabled? (${IsPluginEnabled[mq2berzerker]})
	/echo /echo alliance: is alliance usage enabled? (${IsAllianceEnabled})
	/echo /echo grind: is grind enabled? (${IsGrindEnabled})
	/echo /echo ae: is AE skill usage enabled? (${IsAEEnabled})	
/return

sub ber
	/if (${IsPluginEnabled[mq2berzerker]}) {
		/zerk burnalways off
		/zerk mode manual
	}
:Main
	/if (${Me.Dead}) /endmacro
	/if (${Me.Ducking}) {
		/delay 1s
		/doevents
		/goto :Main
	}
	/call Logic
	/call Alliance
	/call CombatSkills
	/call Meditate
	
	/call IdleSelfBuffs
	/doevents
	/delay 4
	/doevents
	/goto :Main
/return

Sub Event_Grind
	/varcalc IsGrindEnabled !${IsGrindEnabled}
	/echo Grind is now ${IsGrindEnabled}
/return

Sub Event_Alliance
	/varcalc IsAllianceEnabled !${IsAllianceEnabled}
	/echo Alliance mode is now ${IsAllianceEnabled}
/return


Sub Event_AE
	/varcalc IsAEEnabled !${IsAEEnabled}
	/echo Will I do AE skills? ${IsAEEnabled}
/return


Sub CombatSkills
	/if (${IsPluginEnabled[mq2berzerker]}) /return
	/if (!${Target.ID}) /return
	/if (!${Target.Type.Equal[NPC]}) /return
	/if (${Target.PctHPs}==100) /return
	
	/if (${Me.Invis}) /return
	/if (${Target.Distance}>40) /return
	/if (${Me.PctEndurance}<2) /return

	
	/varset SpellName Sucker Punch Rk. II
	/if (${Me.CombatAbilityReady[${SpellName}]} && ${IsOutOfCombat[]}) {
		/doability "${SpellName}"
		/casting "${SpellName}"
		/dgtell ${SpellName} ${Me.XTarget}e
	}

	/varset SpellName Binding Axe
	/if (${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}
	
	/if (${Me.AbilityReady[Intimidation]}) /doability Intimidation
	

	| 10m it16
	/varset SpellName Battleworn Stalwart Moon Warmonger Coat
	/if (!${Defined[BpTimer]}) /declare BpTimer timer outer
	/if (!${BpTimer} && ${Target.PctAggro}>=100 && ${Me.PctHPs}<60 && ${Cast.Ready[${SpellName}]}) {
		/varset BpTimer 600s
		/casting "${SpellName}"
		/echo bp click defensive
		/return
	}

	/varset SpellName Battle Focus Discipline
	/if (${Me.CombatAbilityReady[${SpellName}]} && ${Target.PctAggro}>=100 && ${Me.PctHPs}<60) {
		/if (${Me.ActiveDisc.Name.Length} && ${Me.ActiveDisc.Name.NotEqual[${SpellName}]}) {
			/echo stopping ${Me.ActiveDisc.Name}
			/stopdisc
		}
		
		/doability "${SpellName}"
		/dgtell battle focus time.. evading melee ${Me.PctHPs}%
		/return
	}
	

	/if (!${Defined[BpTimer]}) /declare BpTimer timer outer
	/varset SpellName Adament Triumphant Cloud Warmonger Coat
	/if (${Target.PctAggro}>=100 && ${BurnID}==${Target.ID} && !${BpTimer}) {
		/dgtell chest defensive
		/casting "${SpellName}"
		/varset BpTimer 600s
	}

	
	| 10m t16
	/varset SpellName Blood Sustenance
	/if (${Cast.Ready[${SpellName}]} && ${Target.PctAggro}>=100) {
		/casting "${SpellName}"
		/dgtell ${SpellName} ${Me.PctHPs}% ${Me.XTarget}e
		/return
	}

	
	| 10m aat14 Juggernaught's Resolve
	/varset SpellName Juggernaught's Resolve
	/if (${Cast.Ready[${SpellName}]} && ${Target.PctAggro}>=100) {
		/casting "${SpellName}"
		/dgtell ${SpellName} ${Me.PctHPs}%
		/return
	}

	/varset SpellName Battle Focus Discipline
	/if (${Me.CombatAbilityReady[${SpellName}]} && ${Target.PctAggro}>=100 && ${Me.PctHPs}<50) {
		/if (${Me.ActiveDisc.Name.Length} && Me.ActiveDisc.Name.NotEqual[${SpellName}]) {
			/echo stopping ${Me.ActiveDisc.Name}
			/stopdisc
		}
		
		/doability "${SpellName}"
		/dgtell battle focus time.. evading melee ${Me.PctHPs}%
		/return
	}
	
	| 3m aa8 Uncanny Resilience
	/varset SpellName Uncanny Resilience
	/if (${Me.PctHPs}<90 && ${Cast.Ready[${SpellName}]} && ${Target.PctAggro}>=100) {
		/casting "${SpellName}"
		/dgtell Uncanny Resilience
		/return
	}

	/call IsBuffedWithHealOverTime
	| 1s t1
	/varset SpellName Bloodfury
	/if (!${Defined[BloodFuryTimer]}) /declare BloodFuryTimer timer outer
	/if (${Me.Level}==110 && !${BloodFuryTimer} && ${Macro.Return} && ${Me.PctHPs}>95 && ${Cast.Ready[${SpellName}]} && ${Me.Song[Bloodfury V].ID}==0 && ${Me.Buff[Bloodshield III].ID}==0) {
		/varset BloodFuryTimer 30s
		/casting "${SpellName}"
		/echo ${SpellName}
		/return
	}

	


	| 1s t1
	|blocked by cleaving acrimony discipline
	/varset SpellName Cry Carnage
	/if (${Me.CombatAbilityReady[${SpellName}]} && ${Me.Song[Cry Carnage].ID}==0 && ${Me.ActiveDisc.Name.Length}==0) {
		/doability "${SpellName}"
		/echo ${SpellName}
		/return
	}



	| 2m t11
	/varset SpellName Bolstered Frenzy Rk. II
	/if (!${Defined[BolsteredTimer]}) /declare BolsteredTimer timer outer
	/if (!${BolsteredTimer} && ${Me.PctHPs}<90 && ${Me.CombatAbilityReady[${SpellName}]}) {
		/varset BolsteredTimer 60s
		/doability "${SpellName}"
		/echo ${SpellName} (MyHP: ${Me.PctHPs}%)
		/return
	}


	| 10m t15
	/varset SpellName Frenzied Resolve Discipline
	/if (!${Defined[FrenziedTimer]}) /declare FrenziedTimer timer outer
	/if (!${FrenziedTimer} && ${IsGrindEnabled} && ${Me.CombatAbilityReady[${SpellName}]} &&  ${Me.ActiveDisc.Name.Length}==0 && ${Me.PctHPs}<90) {
		/varset FrenziedTimer 30s
		/doability "${SpellName}"
		/echo ${SpellName} (MyHP: ${Me.PctHPs}%)
		/return
	}

	/if (${Me.ActiveDisc.Name.Equal[${SpellName}]} && ${Target.Distance}>15) {

	}


	| 12s t17
	/varset SpellName Axe of Empyr Rk. II
	/if (${FindItemCount[=Axe of the Mangler]}>0 && (${Target.ID}==${BurnID} || (${IsGrindEnabled} && ${Me.PctEndurance}>50)) && ${Me.CombatAbilityReady[${SpellName}]} && ${Me.Song[${SpellName}].ID}==0) {
		/doability "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}

	| 1s t1 drops to below 90% hp
	/varset SpellName Bloodfury
	/if (${Me.CombatAbilityReady[${SpellName}]} && ${Me.Buff[Bolstered Frenzy].ID}==0 && ${Me.PctHPs}>=90 &&  ${Me.CombatAbilityReady[Bolstered Frenzy]}) {
		/doability "${SpellName}" 
		/echo ${SpellName} (MyHP: ${Me.PctHPs}%) to prime Bolstered Frenzy
	}

	| 1s t1 drops to below 90% hp
	/varset SpellName Bloodfury
	/if (${Me.CombatAbilityReady[${SpellName}]} && ${Me.ActiveDisc.Name.Length}==0 && ${Me.PctHPs}>=90 &&  ${Me.CombatAbilityReady[Frenzied Resolve Discipline]}) {
		/doability "${SpellName}" 
		/echo ${SpellName} (MyHP: ${Me.PctHPs}%) to prime Frenzied Resolve
	}


	| 1s t1 drops to below 90% hp
	/varset SpellName Bloodfury
	/if (${BurnID}==${Target.ID} && ${Me.CombatAbilityReady[${SpellName}]} && ${Me.Buff[Proactive Retaliation].ID}==0 && ${Me.Buff[Proactive Retaliation Rk. II].ID}==0 && ${Me.PctHPs}>=90 &&  ${Me.CombatAbilityReady[Proactive Retaliation]}) {
		/doability "${SpellName}" 
		/echo ${SpellName} (MyHP: ${Me.PctHPs}%) to prime Proactive Retaliation
	}


	
	| item clicky
	/varset SpellName Miniature Horn of Unity
	/if (${Cast.Ready[${SpellName}]} && ${Me.Song[Blessing of Unity].ID} == 0 && ${Me.PctEndurance}<100) {
		/casting "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}

	
	| 10m bp
	/varset SpellName Cohort's Warmonger Coat
	/if (${Target.ID}==${BurnID} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName}
		/return
	}


	| 10m aat18
	/varset SpellName Reckless Abandon
	/if (${BurnID}==${Target.ID} && ${Cast.Ready[${SpellName}]} && !${Me.ActiveDisc.Name.Length} && !${Me.ActiveDisc.Name.Equal[Reckless Abandon]}) {
		/casting "${SpellName}" 
		/echo burn ${SpellName}
	}

	| 30s t12
	/varset SpellName Mangling Frenzy Rk. II
	/if (${Target.Level}>100 && ${Target.Buff[Overpowering Frenzy Effect].ID}==0 && ${Me.CombatAbilityReady[${SpellName}]}) {
		/doability "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}

	
	| 10s t10
	/varset SpellName Mangling Axe Throw Rk. II
	/if (${FindItemCount[=Axe of the Mangler]}>0 && ${Me.PctEndurance} > 50 && ${Me.CombatAbilityReady[${SpellName}]}) {
		/doability "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}

	| 12s t2 de-aggro?
	|/varset SpellName Jarring Jolt
	|/if (${Me.CombatAbilityReady[${SpellName}]} && ${Me.Song[${SpellName}].ID}==0) {
	|	/doability "${SpellName}"
	|	/echo ${SpellName}
	|	/return
	|}

	| 12s t2
	/varset SpellName Mangling Volley Rk. II
	/if (${Me.CombatAbilityReady[${SpellName}]} && ${Me.Song[${SpellName}].ID}==0) {
		/doability "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}

	| 1m t7
	/varset SpellName Smoldering Rage Rk. II
	/if (${Me.CombatAbilityReady[${SpellName}]} && ${Me.Song[${SpellName}].ID}==0) {
		/doability "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}

	| 2.5m t8
	/varset SpellName Sapping Strikes
	/if (${Me.CombatAbilityReady[${SpellName}]}) {
		/doability "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}

	| 1m t14
	/varset SpellName Shared Ruthlessness
	/if (${Me.CombatAbilityReady[${SpellName}]} && ${Me.Song[Reflected Ruthlessness].ID}==0) {
		/doability "${SpellName}"
		/echo ${SpellName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}


	| 12s t16
	|/varset SpellName Arcsteel
	|/if (${IsAEEnabled} && (${Target.ID}==${BurnID} || (${IsGrindEnabled} && ${Me.PctEndurance}>50)) && ${Me.CombatAbilityReady[${SpellName}]} && ${Me.Song[${SpellName}].ID}==0) {
	|	/doability "${SpellName}"
	|	/echo ${SpellName}
	|	/return
	|}

	| 1m t16
	/varset SpellName Vicious Cycle Rk. II
	/if (${IsAEEnabled} && ${NearbyXTargets[]}>=2 && ${Me.CombatAbilityReady[${SpellName}]}) {
		/doability "${SpellName}"
		/dgtell AE ${SpellName} ${Me.XTarget}e
		/return
	}

	| 2m t1
	/varset SpellName Rampage
	/if (${IsAEEnabled} && ${NearbyXTargets[]}>=2 && (${Target.ID}==${BurnID} || (${IsGrindEnabled} && ${Me.PctEndurance}>50)) && ${Cast.Ready[${SpellName}]} && ${Me.Song[${SpellName}].ID}==0) {
		/casting "${SpellName}"
		/dgtell ${SpellName}
		/return
	}
	
	| 2m t19
	/varset SpellName Phantom Assailant
	/if (${Me.CombatAbilityReady[${SpellName}]} && ${Target.PctHPs}>50) {
		/doability "${SpellName}"
		/echo ${SpellName}
		/return
	}


	| 30m t9
	/declare battleCryName string local Battle Cry of the Mastruq
	/if (${Target.ID}==${BurnID} && ${Me.ActiveDisc.Name.Length}==0 && ${Me.CombatAbilityReady[${battleCryName}]} && ${Me.Song[${battleCryName}].ID}==0) {
		/doability "${battleCryName}"
		/echo ${SpellName} -> ${Target.CleanName}
		/delay 5
	}

	| 1m aat63
	/varset SpellName War Cry of the Braxi
	/if (${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName}
		/return
	}

	/call DoDissidentRage
	
	| 20s aat13
	/varset SpellName Battle Leap
	/if (${Cast.Ready[${SpellName}]} && ${Me.Song[Battle Leap Warcry].ID}==0) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName}
		/varset GrindBurnID ${Target.ID}
		/return
	}

	| 14m aat71
	/varset SpellName Agony of Absolution
	/if ((${IsGrindEnabled} && ${GrindBurnID}!=${Target.ID} || ${Target.ID}==${BurnID}) && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName}		
		/varset GrindBurnID ${Target.ID}
		/return
	}

	| 6m aat41
	/varset SpellName Juggernaut Surge
	/if (${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName}
		/return
	}

	| 5m
	/varset SpellName Vehement Rage
	/if (${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName}
		/return
	}

	| 10m aat7
	/varset SpellName Blinding Fury
	/if ((${IsGrindEnabled} && ${GrindBurnID}!=${Target.ID} || ${Target.ID}==${BurnID}) && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName}
		/varset GrindBurnID ${Target.ID}
		/return
	}

	| 10m aa24
	/varset SpellName Savage Spirit
	/if ((${IsGrindEnabled} && ${GrindBurnID}!=${Target.ID} || ${Target.ID}==${BurnID}) && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName}
		/varset GrindBurnID ${Target.ID}
		/return
	}

	| 12m aat4
	/varset SpellName Blood Pact
	/if ((${IsGrindEnabled} && ${GrindBurnID}!=${Target.ID} || ${Target.ID}==${BurnID}) && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName}
		/varset GrindBurnID ${Target.ID}
		/return
	}



	
	| 12m aat18
	|/varset SpellName Conditioned Retaliation
	|/if (!${Defined[ConditionedTimer]}) /declare ConditionedTimer timer outer
	|/if (!${ConditionedTimer} && ${Target.ID}==${BurnID} && ${Me.ActiveDisc.Name.Length}==0 && ${Cast.Ready[${SpellName}]}) {
	|	/varset ConditionedTimer 30s
	|	/casting "${SpellName}"
	|	/echo ${SpellName} -> ${Target.CleanName}
	|	/return
	|}

	| 21m aat2
	/varset SpellName Untamed Rage
	/if (${Target.ID}==${BurnID} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName}
		/return
	}

	| 20m aat11 single target ramp
	|/varset SpellName Focused Furious Rampage
	| 20m aat11 ae ramp
	/varset SpellName Furious Rampage
	/if (${IsAEEnabled} && ${NearbyXTargets[]}>=2 &&  && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/dgtell furious rampage ${SpellName} -> ${Target.CleanName}
		/return
	}

	/varset SpellName Focused Furious Rampage
	/if (!${IsAEEnabled} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName}
		/return
	}

	| 7m aat40
	/varset SpellName Spire of the Juggernaught
	/if (${Target.ID}==${BurnID} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo Burn ${SpellName} -> ${Target.CleanName}
		/return
	}

	| 21m aat5
	/varset SpellName Desperation
	/if (${Target.ID}==${BurnID} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo Burn ${SpellName} -> ${Target.CleanName}
		/return
	}

	| 10m
	/varset SpellName Boreal Coat of Carnage
	/if (${Target.ID}==${BurnID} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo Burn ${SpellName} -> ${Target.CleanName}
		/return
	}

	/if (${IsGrindEnabled} && ${Me.XTarget}>1 && ${Me.ActiveDisc.Name.Length}==0) {
		| 3m t21
		/varset SpellName Disconcerting Discipline Rk. II
		/if (${Me.CombatAbilityReady[${SpellName}]}) {
			/doability "${SpellName}"
			/echo grind ${SpellName} ${Me.XTarget}e
			/return
		}	
	}

	/if (${Target.ID}==${BurnID} && ${Me.ActiveDisc.Name.Length}==0) {
		
		| 25m t4
		/varset SpellName Brutal Discipline
		/if (!${Defined[BrutalTimer]}) /declare BrutalTimer timer outer
		/if (!${BrutalTimer} && ${Me.CombatAbilityReady[${SpellName}]}) {
			/varset BrutalTimer 30s
			/doability "${SpellName}"
			/echo Burn ${SpellName} -> ${Target.CleanName}
			/return
		}

		| 20m t3
		/varset SpellName Cleaving Acrimony Discipline
		/if (${Me.CombatAbilityReady[${SpellName}]}) {
			/doability "${SpellName}"
			/echo Burn ${SpellName} -> ${Target.CleanName}
			/return
		}	

		| 3m t21
		/varset SpellName Disconcerting Discipline Rk. II
		/if (${Me.CombatAbilityReady[${SpellName}]}) {
			/doability "${SpellName}"
			/echo Burn ${SpellName} -> ${Target.CleanName}
			/return
		}	

		| 15m t6
		/varset SpellName Avenging Flurry Discipline
		/if (${Me.CombatAbilityReady[${SpellName}]}) {
			/doability "${SpellName}"
			/echo Burn ${SpellName} -> ${Target.CleanName}
			/return
		}	

	}
	
/return

Sub Alliance
	/if (!${IsAllianceEnabled}) /return

	/varset SpellName Mangler's Covenant Rk. II
	/if (!${Me.CombatAbilityReady[${SpellName}]}) {
		/varset SpellName Mangler's Alliance
		/if (!${Me.CombatAbilityReady[${SpellName}]}) /return
		/return
	}

	/if (${Me.Animation}==16) /return
	/if (!${Me.Standing}) /return
	/if (${Me.XTarget}==0) /return
	/if (${Window[CastingWindow]}) /return
	/if (${Target.Distance} > 30) /return
	/if (${Me.Moving}) /return
	/if (!${Target.Type.Equal[NPC]}) /return	
	/if (!${Target.LineOfSight}) /return

	/if (${Target.ID} != ${BurnID}) {
		/if (${Me.PctEndurance}<70) /return
		/if (${Target.PctHPs}<30) /return

	}

	/if (${IsAllianceOnMob[]}) /return
	
	/if (${Target.PctHPs}>70 && ${Me.PctEndurance}>50 && ${Me.CombatAbilityReady[${SpellName}]}) {
		/doability "${SpellName}"
		|/g Casting xx Berserker's Alliance xx
		/dgtell alliance
		/return
	}
/return


Sub IsAllianceOnMob
	/varset SpellName Mangler's Alliance
	/if (${Target.Buff[${SpellName}].ID}>0 && ${Target.Buff[${SpellName}].Caster.NotEqual[${Me.Name}]}) /return true
	/varset SpellName Mangler's Alliance Rk. II
	/if (${Target.Buff[${SpellName}].ID}>0 && ${Target.Buff[${SpellName}].Caster.NotEqual[${Me.Name}]}) /return true
	/varset SpellName Mangler's Alliance Rk. III
	/if (${Target.Buff[${SpellName}].ID}>0 && ${Target.Buff[${SpellName}].Caster.NotEqual[${Me.Name}]}) /return true

	/varset SpellName Mangler's Covenant
	/if (${Target.Buff[${SpellName}].ID}>0 && ${Target.Buff[${SpellName}].Caster.NotEqual[${Me.Name}]}) /return true
	/varset SpellName Mangler's Covenant Rk. II
	/if (${Target.Buff[${SpellName}].ID}>0 && ${Target.Buff[${SpellName}].Caster.NotEqual[${Me.Name}]}) /return true
	/varset SpellName Mangler's Covenant Rk. III
	/if (${Target.Buff[${SpellName}].ID}>0 && ${Target.Buff[${SpellName}].Caster.NotEqual[${Me.Name}]}) /return true
/return false


Sub DoReactDefensive
	/if (${Me.Animation}==16) /return
	/if (!${Me.Standing}) /return
	/if (${Me.XTarget}==0) /return
	/if (${Me.Casting.ID}) /return

	/if (${IsGrindEnabled} && ${Me.XTarget}>1 && ${Me.ActiveDisc.Name.Length}==0) {
		| timer 7 defensive
		/varset SpellName Capricious Rage Discipline
		/if (${Me.CombatAbilityReady[${SpellName}]}) {
			/doability "${SpellName}"
			/echo Burn ${SpellName} -> ${Target.CleanName}
			/return
		}

	}

	/if (${Me.PctHPs}>60) /return
	
	/if (${Me.PctHPs}>30) /return
	/if (${Cast.Ready[${SpellName}]}) /casting "${SpellName}"
	
	/if (${Me.PctMana}<30) /return
	/if (${Me.Moving}) /return

/return

Sub IdleSelfBuffs

	/varset SpellName Frenzied Resolve Discipline
	/if (${Navigation.Active} && ${Me.ActiveDisc.Name.Equal[${SpellName}]}) {
		/echo stopping ${SpellName}
		/stopdisc
	}

	/if (${Me.Animation}==16) /return
	/if (${Me.XTarget}>0) /return
	/if (${Me.Casting.ID}) /return
	/if (${Me.Moving}) /return
	/if (${Me.Invis}) /return
	/if (${Me.Buff[Illusion: Dark Elf].ID} == 0) {
		/casting "Guise of the Deceiver" -invis
		/return
	}

	/if (${Me.CombatState.Equal[COOLDOWN]}) {
		/varset SpellName Communion of Blood
		/if (${Cast.Ready[${SpellName}]} && ${Me.PctEndurance}<80) {
			/casting "${SpellName}"
			/echo ${SpellName} ${Me.PctEndurance}% endurance and in cooldown state
		}
	}

	/if (${ComponentNagTimer}==0 && ${FindItemCount[=Masterwork Axe Components]}<20) {
		/dgtell Need Masterwork Axe Components!
		/varset ComponentNagTimer 10m
	}

	
	/varset SpellName Axe of the Mangler

	/if (${FindItemCount[=Masterwork Axe Components]}>0 && ${FindItemCount[=${SpellName}]}<100 && ${Me.CombatAbilityReady[${SpellName}]} && ${Me.PctEndurance}>21) {
		/doability "${SpellName}
		/echo Summoning ${SpellName}
		/delay 8s
		/autoinv
		/return
	}

	
	| 1m30s t13 end regen
	/varset SpellName Breather
	/if (${Me.CombatAbilityReady[${SpellName}]} && ${Me.PctEndurance}<21) {
		/doability "${SpellName}"
		/echo ${SpellName} ${Me.PctEndurance}%
		/return
	}


	/varset SpellName Miniature Horn of Unity
	/if (${Cast.Ready[${SpellName}]} && ${Me.Song[Blessing of Unity].ID} == 0 && ${Me.PctEndurance}<100) {
		/casting "${SpellName}"
		/return
	}
/return

| /echo ${Window[RewardSelectionWnd]}
| /echo ${Window[RewardSelectionWnd].Child[RewardPageTabWindow].Text} ?
| /echo ${Window[RewardSelectionWnd].Child[RewardSelectionItemList].List[1,2]} ?
| /echo ${Window[RewardSelectionWnd].Child[RewardSelectionDescriptionArea].Text} ?

Sub DoDissidentRage
	/declare epicName string local Vengeful Taelosian Blood Axe
	/declare epicBuff string local Strike of Savagery

	/if (${FindItemCount[=Axe of the Mangler]}<5) /return
	


	/if (${Me.Buff[Nettlespear Reflection].ID}==0 && ${Me.Song[Prophet's Gift of the Ruchu].ID}>0 && ${Cast.Ready[${epicName}]}) {	
		/echo Epic click
		/casting "${epicName}"
	}

	| 12s t22
	/varset SpellName Dissident Rage
	/if (${Me.PctEndurance}<80 && ${Target.ID} != ${BurnID}) /return
	/if (${Me.Song[${epicBuff}].ID}==0) /return
	/if (!${Me.CombatAbilityReady[${SpellName}]}) /return

	/doability "${SpellName}"
	/dgtell ${SpellName} -> ${Target.CleanName}
/return

Sub IsBuffedWithHealOverTime
	/if (${Me.Song[Cloud of Renewal].ID}>0) /return TRUE
 	/if (${Me.Song[Eyrzekla's Watch].ID}>0) /return TRUE
	/if (${Me.Song[Eyrzekla's Persistence].ID}>0) /return TRUE
	/if (${Me.Song[Eyrzekla's Alleviation].ID}>0) /return TRUE
	/if (${Me.Song[Eyrzekla's Forbearance].ID}>0) /return TRUE
/return FALSE

Sub Meditate
	/if (${Stick.Status.Equal[ON]}) /return
	/if (${Navigation.Active}) /return
	/if (!${Me.CombatState.Equal[ACTIVE]} && !${Me.CombatState.Equal[RESTING]}) /return
	/if (${NearbyXTargets[]}>0) /return
	/if (${Window[CastingWindow]}) /return
	/if (${Me.SpellInCooldown}) /return
	/if (${Me.PctEndurance}>90 && ${Me.PctHPs}>90) /return
	
	/if (${MeditateTimer}>0) /return
	/if (${Me.Moving}) {
		/echo interrupted meditate
		/varset MeditateTimer 60s
		/return
	}
	/if (${Me.XTarget} != 0) /return	
	/if (!${Me.Sitting}) {
		/sit
		/varset MeditateTimer 30s
	}
/return