#event BlockedSpell "Your *1* spell did not take hold on #*#"
#event NotMezzable "Your target cannot be mesmerized."
#event Evade "[MQ2] evade"
#event Mez "[MQ] mez"

sub brdInit
	/if (!${Defined[IsEvadeEnabled]}) /declare IsEvadeEnabled bool outer FALSE
	/if (!${Defined[IsMezzingEnabled]}) /declare IsMezzingEnabled bool outer TRUE
	/if (!${Defined[IsMezzing]}) /declare IsMezzing bool outer FALSE
	/if (!${Defined[BurnID]}) /declare BurnID int outer
	/if (!${Defined[IsSongJustStopped]}) /declare IsSongJustStopped bool outer FALSE
	/if (!${Defined[LastSongName]}) /declare LastSongName string outer
	/if (!${Defined[LastSongTimer]}) /declare LastSongTimer timer outer
	/if (!${Defined[MeditateTimer]}) /declare MeditateTimer timer outer
/return

sub brdHelp
	/echo -- ${Me.Class.Name} Commands --
	/echo /echo evade: use fading memories when hit? (${IsEvadeEnabled})
	/echo /echo mez: try to mez mobs? (${IsMezzingEnabled})
/return

Sub brd
:Main
	/if (${Me.Dead}) /endmacro

	/if (${Me.Ducking}) {
		/delay 1s
		/doevents
		/goto :Main
	}

	/if (!${Window[CastingWindow]} && ${Me.Casting.ID}) {
		/if (!${Me.Casting.Name.Equal[Blessing of Unity]}) {
			/stopcast
		}
	}

	/call Fade
	/call AEMez
	/call Mez
	/if (!${IsMezzing} && !${EverQuest.Foreground}) /call Engage
	
	/call CombatSongs
	/call CombatAAs
	/call IdleSelfBuffs
	
	/call Meditate	
	/doevents
	/delay 5
	/doevents
	/goto :Main
/return

Sub Event_NotMezzable
	/if (!${Defined[SingleTargetMezWasUsed]}) /declare SingleTargetMezWasUsed bool outer FALSE
	/if (!${SingleTargetMezWasUsed}) /return
	/echo got message target not mezzable? ${SingleTargetMezTarget}
	/if (!${Ini[mezzable_npcs.ini,${Zone.ShortName},${SingleTargetMezTarget}].Equal[1]}) {
		/ini "mezzable_npcs.ini" "${Zone.ShortName}" "${SingleTargetMezTarget}" "1"
		/echo Added ${npc} to mez immune list
	}
/return

Sub IsMezzable(string npc)
	/if (${Ini[mezzable_npcs.ini,${Zone.ShortName},"${npc}"].Equal[1]}) /return FALSE
/return TRUE

Sub AEMez
	/if (${EverQuest.Foreground}) /return
	/if (!${Defined[KillTarget]}) /declare KillTarget string outer
	/if (${KillTarget.Length}) /return
	/if (${Me.Animation}==16) /return
	/if (!${Me.Standing}) /return
	|/if (${NearbyXTargets[]}<2) /return
	/if (${Me.XTarget}<2) /return
	/if (${Window[CastingWindow]}) /return

	/varset SpellName Wave of Sleep Rk. II
	/if (${Me.GemTimer[${SpellName}]}) /return

	|/if (${Zone.Name.Equal[Gnome Memorial Mountain: Under Assault]} && ${SpawnCount[radius 100 npc an Iron]}>0) /echo Waiting to AE until gnome paths by
	
	/declare aggroNeeded bool local
	/declare aggroCount int local
	/declare i int local

	/for i 1 to ${Me.XTarget}		
		/if (${Me.XTarget[${i}].Distance} > 50) /continue
		/if (!${Me.XTarget[${i}].LineOfSight}) /continue
		/if (${Me.XTarget[${i}].Type.NotEqual[NPC]}) /continue
		/varcalc aggroCount ${aggroCount}+1
	/next i
	
	/if (${aggroCount} < 3) /return

	/if (${Me.Casting.Name.Equal[${SpellName}]}) /return
	|/stopsong
	|/delay 5
	/casting "${SpellName}" -maxtries|3
	/varset LastSongTimer 4s
	/varset LastSongName ${SpellName}
	/dgtell AE Mez ${Me.XTarget}e total, ${aggroCount} within 100m
/return


Sub Mez
	/if (${EverQuest.Foreground}) /return
	/if (!${Defined[KillTarget]}) /declare KillTarget string outer
	/if (${KillTarget.Length}) /return
	/if (!${Defined[LastMezID]}) /declare LastMezID int outer 2
	/if (!${Defined[MezTimer]}) /declare MezTimer timer outer
	/if (!${Defined[SingleTargetMezWasUsed]}) /declare SingleTargetMezWasUsed bool outer FALSE
	/if (!${Defined[SingleTargetMezTarget]}) /declare SingleTargetMezTarget string outer
	/if (${Window[CastingWindow]}) /return
	|/if (${Cast.Status.Equal[C]}) /return
	
	/if (${MezTimer}) /return


	/declare aeMezName string local Wave of Sleep Rk. II
	/declare singleMezName string local Slumber of Zburator
	/declare charmName string local Voice of Jembel Rk. II

	/varset IsMezzing FALSE
	/if (${Me.Casting.Name.Equal[${aeMezName}]}) {
		/varset IsMezzing TRUE
		/echo waiting on ae mez cast
		/return
	}
	/if (${Me.Casting.Name.Equal[${singleMezName}]}) {
		/echo waiting on single mez cast
		/varset IsMezzing TRUE
		/return
	}

	/if (${Me.Casting.Name.Equal[${charmName}]}) {
		/echo waiting on single mez cast
		/varset IsMezzing TRUE
		/return
	}
	

	
	/if (${Group.MainTank.Distance}>100) /return
	/if (!${Group.MainTank.LineOfSight}) /return
	

	/if (${Me.XTarget}==0) /varset LastMezID 1		
	
	/if (!${Me.Standing}) /return	

	/if (${LastMezID} >= ${Me.XTarget}) /varset LastMezID 1
	/varset MezTimer 8s

	/echo mez pulse check
	/declare i int local
	
	/stick off
	|/stopsong
	/for i ${LastMezID} to ${Me.XTarget}	
		/if (!${Me.XTarget[${i}].ID}) /continue
		/if (${Me.XTarget[${i}].PctHPs} < 80) /continue
		/if (${Me.XTarget[${i}].Distance} > 150) {
			/echo ${i} ${Me.XTarget[${i}].CleanName} too far
			/continue
		}
		/if (${Me.XTarget[${i}].Type.NotEqual[NPC]}) /continue
		/if (!${Me.XTarget[${i}].LineOfSight}) {
			/echo ${i} ${Me.XTarget[${i}].CleanName} not line of sight
			/continue
		}
		/if (!${IsMezzable["${Me.XTarget[${i}].CleanName}"]}) {
			/echo ${i} ${Me.XTarget[${i}].CleanName} not mezzable gonna try anyways

			/continue
		}

		
		/if (${Me.Combat}) {
			/echo turning off attack for mezzing
			/attack
			/delay 5
		}
		/echo mez target
		/target id ${Me.XTarget[${i}].ID}
		/delay 1s
		
		/delay 1s ${Target.BuffsPopulated}
		/varset SpellName ${singleMezName}
		/if (${Target.Buff[${SpellName}].ID} && ${Target.BuffDuration[${SpellName}]}>=10000) {
			/echo ${i} ${Target.CleanName} OK: single mez ${Target.BuffDuration[${SpellName}]}ms left
			/continue
		}

		/varset SpellName ${aeMezName}
		/if (${Target.Buff[${SpellName}].ID} && ${Target.BuffDuration[${SpellName}]}>=10000) {
			/echo ${i} ${Target.CleanName} OK: ae mez ${Target.BuffDuration[${SpellName}]}ms left
			/continue
		}

		/varset SpellName ${charmName}
		/if (${Target.Buff[${SpellName}].ID} && ${Target.BuffDuration[${SpellName}]}>=10000) {
			/echo ${i} ${Target.CleanName} OK: charm ${Target.BuffDuration[${SpellName}]}ms left
			/continue
		}


		/varset SpellName ${singleMezName}
		/if (!${Me.GemTimer[${aeMezName}]}) /varset SpellName ${aeMezName}

		/stopcast
		/delay 5
		/cast ${Me.Gem[${SpellName}]}
		
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/dgtell ${SpellName} on #${i} ${Me.XTarget[${i}].CleanName}
		/if (${SpellName.Equal[${singleMezName}]}) {
			/varset SingleTargetMezWasUsed true
			/varset SingleTargetMezTarget ${Target.CleanName}
		}
		/delay 4s ${Target.Buff[${SpellName}].ID}
		/echo should be mezzed?
		/doevents
		/varset SingleTargetMezWasUsed false
		/echo on to next mob
	/next i
	/delay 5
	/echo resuming combat
	/target id ${Me.XTarget[1].ID}
	/if (!${Me.Combat} && ${IsTargetLegit[]}) {
		/echo turning on attack after mezzing
		/attack
	}
	
/return

Sub Fade
	/varset SpellName Fading Memories
	/if (!${Defined[FadeTimer]}) /declare FadeTimer timer outer
	/if (!${Defined[IsEvadeEnabled]}) /declare IsEvadeEnabled bool outer true
	/if (!${IsEvadeEnabled}) /return

	/if (${Group} && ${Me.PctHPs}<80 && !${FadeTimer} && ${Me.PctMana}>10) {
		/varset FadeTimer 10s
		/alt activate 212
		/dgtell Fading aggro
		/return
	}
/return

Sub CombatAAs
	/if (${Me.Invis}) /return
	/if (!${Me.XTarget}) /return
	/if (${Me.PctEndurance}<2) /return
	/if (${Target.PctHPs}>99) /return
	
	/call Selos


	| 7.5m aat40
	/varset SpellName Spire of the Minstrels
	/if (${Target.ID} && ${BurnID}==${Target.ID} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo burn ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}


	| 7.5m aat40
	/varset SpellName Vainglorous Shout
	/if (${Me.XTarget}<2 && ${Target.ID} && ${BurnID}==${Target.ID} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/dgtell shouting since only 1 enemy
		/return
	}

	| 10m aat16
	|/varset SpellName Quick Time
	|/if (${Target.ID} && ${BurnID}==${Target.ID} && ${Cast.Ready[${SpellName}]}) {
	|	/casting "${SpellName}"
	|	/echo burn ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
	|	/return
	|}


	| 9m aat32
	/varset SpellName Funeral Dirge
	/if (${Target.ID} && ${BurnID}==${Target.ID} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo burn ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}

	/if (!${Me.Song[Fierce Eye].ID} && !${Me.Song[Spirit of Vesagran].ID}) {	
		/varset SpellName Fierce Eye
		/if (${Me.XTarget}>1 && ${Cast.Ready[${SpellName}]}) {
			/casting "${SpellName}"
			/echo ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e			
			/echo AA click
			/return
		}
	}

	/varset SpellName Blade of Vesagran
	/if (!${Me.Song[Fierce Eye].ID} && !${Me.Song[Spirit of Vesagran].ID} && ${Cast.Ready[${SpellName}]}) {		
		/if (${Cast.Ready[${SpellName}]}) {
			/casting "${SpellName}"
			/echo epic click
			/return
		}
	}


	| 12m aat80
	/varset SpellName Flurry of Notes
	/if (${Target.ID} && ${Cast.Ready[${SpellName}]} && ${Target.PctHPs}>80) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}

	| 4m aat9
	/varset SpellName Bladed Song
	/if (${Target.ID} && ${Cast.Ready[${SpellName}]} && ${Target.PctHPs}>90 && ${Target.Distance}<20) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}

	| 12m aat2
	/varset SpellName Dance of Blades
	/if (${Target.ID} && ${BurnID}==${Target.ID} && ${Cast.Ready[${SpellName}]} && ${Target.PctHPs}>90) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}

	| 3m aat6
	/varset SpellName Cacophony
	/if (${Target.ID} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}


	| 20m aat3
	/varset SpellName Shield of Notes
	/if (${Target.ID} && ${BurnID}==${Target.ID} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo burn ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}


	| 12m aat4
	/varset SpellName Song of Stone
	/if (${Target.ID} && ${BurnID}==${Target.ID} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo burn ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}


	| 12s aat1
	/varset SpellName Boastful Bellow
	/if (${Target.ID} && (${BurnID}==${Target.ID} || ${Me.PctEndurance}>90) && ${Cast.Ready[${SpellName}]} && !${Target.Buff[Boastful Bellow XLI].ID}) {
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}

	| 12m aat10
	/varset SpellName Frenzied Kicks
	/if (${BurnID}==${Target.ID} && ${Cast.Ready[${SpellName}]} && ${Target.PctHPs}>80) {
		/casting "${SpellName}"
		/echo burn ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}


	| 2m aat5
	/varset SpellName Lyrical Prankster
	/if (!${Defined[LyricalTimer]}) /declare LyricalTimer timer outer
	/if (!${LyricalTimer} && ${Cast.Ready[${SpellName}]} && ${Target.PctHPs}>70 && ${Target.Distance}<20) {
		/varset LyricalTimer 6s
		/casting "${SpellName}"
		/echo ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}


	| 13m aat48
	/varset SpellName Tune Stuck In Your Head
	/if (${Target.ID} && ${BurnID}==${Target.ID} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo burn ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}


	/if (${Target.ID} && ${Me.AbilityReady[Intimidation]}) /doability Intimidation

/return

Sub IsTargetLegit
	/if (${Target.PctHPs}>99) /return false
	/if (${Target.ID} != ${Me.XTarget[1].ID}) /return false
/return true

Sub CombatSongs
	/if (${IsSongJustStopped}) /return
	|/if (!${Target.Type.Equal[NPC]}) /return
	|/if (${Target.PctHPs}==100) /return
	|/if (!${Me.Combat}) /return
	/if (${Me.Invis}) /return
	|/if (${Target.Distance}>40) /return
	/if (!${Me.XTarget}) /return
	/if (${Me.PctEndurance}<2) /return
	|/if (${Window[CastingWindow]}) /return
	/if (${Me.Casting.ID}) /return

	
	/varset SpellName Kluzen's Spry Sonata Rk. II
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}
	

	/varset SpellName Requiem for the Dead Rk. II
	/if (!${SpellName.Equal[${LastSongName}]} && ${Group.Injured} && !${Me.Song[${SpellName}].ID} && ${Cast.Ready[${SpellName}]}) {
		|/casting "${SpellName}"
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/echo burn ${SpellName}
		/return
	}

	/varset SpellName Ervaj's Defensive Refrain Rk. II
	/if (${Group.MainTank.PctHPs}<40 && ${Group.MainTank.PctMana}>70 && ${Cast.Ready[${SpellName}]}) {
		/dgtell Mana shield
		/cast ${Me.Gem[${SpellName}]}
		/return
	}


	| 10m it16
	/varset SpellName Battleworn Stalwart Moon Loremaster Breastplate
	/if (${Target.ID} > 0 && ${BurnID}==${Target.ID} && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/echo burn ${SpellName} -> ${Target.CleanName} ${Target.PctHPs}% ${Me.XTarget}e
		/return
	}



	/varset SpellName Sontalak's Chant of Flame
	/if (!${Defined[FlameTimer]}) /declare FlameTimer timer outer
	/if (!${SpellName.Equal[${LastSongName}]} && ${IsTargetLegit[]} && !${FlameTimer} && ${Target.ID} && ${Target.Buff[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/varset FlameTimer 18s
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}


	/if (!${Me.Song[Doben's Spry Sonata Rk. II].ID}) {
		/casting "Darkened Breath of Harmony"
		|/echo Doben's Spry Sonata Rk. II
		/return
	}


	/varset SpellName Dissident Psalm Rk. II
	/if (!${SpellName.Equal[${LastSongName}]} && ${IsTargetLegit[]} && (${BurnID}==${Target.ID} || ${Me.PctEndurance}>80) && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/echo dissident psalm
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}

	
	
	/varset SpellName Omorden's Insult Rk. II
	/if (!${Defined[InsultTimer]}) /declare InsultTimer timer outer
	/if (!${SpellName.Equal[${LastSongName}]} && ${IsTargetLegit[]} && !${InsultTimer} && ${Target.ID} && ${Target.PctHPs}<99 && !${Me.Song[Troubadour's Synergy II].ID} && ${Me.PctMana}>70 && ${Cast.Ready[${SpellName}]}) {
		/varset InsultTimer 18s
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}

	/varset SpellName Malvus's Chant of Poison Rk. II
	/if (!${Defined[PoisonTimer]}) /declare PoisonTimer timer outer
	/if (!${SpellName.Equal[${LastSongName}]} && ${IsTargetLegit[]} && !${PoisonTimer} && ${Target.ID} && ${Target.Buff[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/varset PoisonTimer 18s
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}

	
	/varset SpellName Zlexak's Chant of Disease Rk. II
	/if (!${Defined[DiseaseTimer]}) /declare DiseaseTimer timer outer
	/if (!${SpellName.Equal[${LastSongName}]} && ${IsTargetLegit[]} && !${DiseaseTimer} && ${Target.ID} && ${Target.Buff[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/varset DiseaseTimer 18s
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}


	

	/varset SpellName Yelinak's Chant of Frost Rk. II
	/if (!${Defined[FrostTimer]}) /declare FrostTimer timer outer
	/if (!${SpellName.Equal[${LastSongName}]} && ${IsTargetLegit[]} && !${FrostTimer} && ${Target.ID} && ${Target.Buff[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/varset FrostTimer 18s
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}
	

	|/varset SpellName Chorus of Xigam Rk. II
	|/if (!${SpellName.Equal[${LastSongName}]} && !${Me.Song[${SpellName}].ID} && ${Cast.Ready[${SpellName}]}) {
	|	/cast ${Me.Gem[${SpellName}]}
	|	/varset LastSongTimer 4s
	|	/varset LastSongName ${SpellName}
	|	/echo ${SpellName}
	|	/return
	|}


	

	/varset SpellName Travenro's Song of Suffering
	/if (!${SpellName.Equal[${LastSongName}]} && ${IsTargetLegit[]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}


	/varset SpellName Jembel's Lively Crescendo Rk. II
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]} && ${Me.PctMana}>90) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}

	
	/varset SpellName Arcane Ballad Rk. II
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}

	/varset SpellName Arcane Symphony
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}


	/varset SpellName Atoning Accelerando
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}

	

	/varset SpellName Reivaj's Reckless Renewal Rk. II
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}

	/varset SpellName War March of Radiwol
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}


	


	

	/varset SpellName Second Psalm of Veeshan
	/if (${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}

	
	/varset SpellName Dirge of Lost Horizons
	/if (${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}

	/varset SpellName Aria of Margidor Rk. II
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}

	/varset SpellName Omorden's Spiteful Lyric
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}
	
/return

Sub Event_Evade
	/varcalc IsEvadeEnabled !${IsEvadeEnabled}
	/echo doing evade is now ${IsEvadeEnabled}
/return

sub Selos
	/if (${Zone.ShortName.Equal[velketortwo]}) /return
	
	/varset SpellName Selo's Sonata
	/if (!${Defined[SelosTimer]}) /declare SelosTimer timer outer
	|/if ((!${Me.Buff[Selo's Sonata I].ID} || ${Me.Buff[Selo's Sonata I].Duration}<60000) && ${Cast.Ready[${SpellName}]}) {
	/if (!${SelosTimer} && ${Cast.Ready[${SpellName}]}) {
		/varset SelosTimer 30s
		/casting "${SpellName}"
		|/echo ${SpellName}
		/return
	}
/return


Sub IdleSelfBuffs
	/if (${Me.Animation}==16) /return
	/if (${Me.Invis}) /return

	/if (${Me.XTarget}>0) /return	
	/if (${Me.Casting.ID}) /return

	/call Selos
	
	
	/varset SpellName Aura of Margidor
	/if (!${Me.Invis} && ${Me.Song[Aura of Margidor Effect].ID}==0) {
		/if (!${Aura.Remove}) {
			/echo can't remove aura?
		}
		/removeaura ${Me.Aura}
		/casting "${SpellName}" gem13
	}

	/varset SpellName Miniature Horn of Unity
	/if (${Cast.Ready[${SpellName}]} && ${Me.Song[Blessing of Unity].ID} == 0 && ${Me.PctEndurance}<100) {
		/casting "${SpellName}"
		/return
	}
	/varset SpellName Dirge of Lost Horizons
	/if (${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/casting "${SpellName}"
		/return
	}


	/if (${Cast.Ready[${SpellName}]} && !${Me.Song[${SpellName}].ID}) {
		/casting "${SpellName}"
		/return
	}

	
	/varset SpellName Rallying Solo
	/if (${Cast.Ready[${SpellName}]} && ${Me.PctEndurance}<30) {
		/casting "${SpellName}"
		/dgtell Rallying Solo ${Me.PctEndurance}% Endurance
		/return
	}

	/varset SpellName Chorus of Xigam Rk. II
	/if (!${SpellName.Equal[${LastSongName}]} && !${Me.Song[${SpellName}].ID} && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/echo ${SpellName}
		/return
	}

	/varset SpellName Jembel's Lively Crescendo Rk. II
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]} && ${Me.PctMana}>90) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}


	/varset SpellName Travenro's Spiteful Lyric Rk. II
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}
	
	
	
	/varset SpellName Arcane Ballad Rk. II
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}


	/varset SpellName Atoning Accelerando
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}

	/varset SpellName War March of Dekloaz Rk. II
	/if (!${SpellName.Equal[${LastSongName}]} && ${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}

	/varset SpellName Second Psalm of Veeshan
	/if (${Me.Song[${SpellName}].ID}==0 && ${Cast.Ready[${SpellName}]}) {
		/cast ${Me.Gem[${SpellName}]}
		/varset LastSongTimer 4s
		/varset LastSongName ${SpellName}
		/return
	}

	|/varset SpellName Reivaj's Reckless Renewal Rk. II
	|/if (!${Me.Gem[13].Name.Equal[${SpellName}]} && !${Cast.Ready[${SpellName}]}) {
	|	/echo mem ${SpellName}
	|	/casting "${SpellName}" gem13
	|}
/return



Sub Meditate
	/if (!${Defined[MeditateTimer]}) /declare MeditateTimer timer outer
	/if (${Stick.Status.Equal[ON]}) /return
	/if (${Navigation.Active}) /return
	/if (!${Me.CombatState.Equal[ACTIVE]} && !${Me.CombatState.Equal[RESTING]}) /return
	/if (${NearbyXTargets[]}>0) /return
	/if (${MeditateTimer}>0) /return
	/if (${Me.PctMana}>95) /return
	/if (${Me.Moving}) {
		/varset MeditateTimer 60s
		/return
	}
	/if (!${Me.Sitting}) /sit
/return