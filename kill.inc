#Event Kill "[MQ2] kill #1#"


sub Event_Kill(line, killTarget)
	/if (!${Defined[KillTarget]}) /declare KillTarget string outer
	
	/varset KillTarget ${killTarget}|
	/echo now killing ${KillTarget} (kill:event_kill)
	/if (!${Defined[IsFacingEnabled]}) /declare IsFacingEnabled bool outer	
	/posse audio on
	/posse on
	/varset IsFacingEnabled true

	/mapfilter castradius 370
/return

sub Kill
	/if (!${Defined[IsKillNothingUp]}) /declare IsKillNothingUp bool outer
	/if (!${Defined[KillTarget]}) /declare KillTarget string outer
	/if (!${Defined[KillRetryTimer]}) /declare KillRetryTimer timer outer
	/if (!${Defined[KillTargetID]}) /declare KillTargetID int outer
	/if (!${Defined[cn]}) /declare cn string local
	/if (!${Defined[crewIndex]}) /declare crewIndex int local

	/if (!${KillTarget.Length}) /return
	/if (${KillRetryTimer}) /return
	/if (${KillTargetID} && !${Spawn[id ${KillTargetID}].Type.Equal[NPC]}) /varset KillTargetID 0

	/if (${Me.XTarget} && ${Navigation.Active}) {
		/echo cancelling nav, got aggro (kill:kill)
		/nav stop
		/return
	}
	
	/if (${Me.Moving}) /return
	/if (${Window[CastingWindow]} && ${Me.Class.ShortName.NotEqual[BRD]}) /return
	
	/if (${Me.XTarget} && ${Target.ID} && !${IsXTarget[${Target.ID}]}) {
		/echo clearing target to grab nearby
		/target clear
	}

	/for crewIndex 1 to ${Math.Calc[${DanNet.Peers.Count[|]}]}
		/varset total 0
		/varset cn ${DanNet.Peers.Token[${crewIndex},|]}
		/if (!${IsPlayerInGroup[${cn}]}) /continue
		/if (!${Spawn[pc ${cn}].ID}) /continue
		/if (${Spawn[pc ${cn}].Class.ShortName.NotEqual[CLR]}) /continue		
		/call ClericPull ${cn}
		/return
	/next crewIndex
	/call BodyKill
	/call XTargetKill
/return

sub XTargetKill
	/if (${Navigation.Active}) /return

	/if (!${Me.XTarget}) /return
	/declare targetID int local ${NearestXTarget[]}
	/if (!${targetID}) /return
	/if (${Spawn[id ${targetID}].Distance}<15) /return
	/echo xtarget kill
	/nav id ${targetID}
/return

sub ClericPull(string name)
	/if (${Me.XTarget}) {
		/if (!${Me.Class.ShortName.Equal[CLR]} || ${Me.XTarget}==1) /return
	}
	/if (${Navigation.Active}) /return
	/if (!${Defined[LastCombatTimer]}) /declare LastCombatTimer timer outer
	/declare targetID int local

	/if (!${Defined[dout]}) /declare dout string local
	/dquery ${cn} -q "Cast.Ready" -o dout
	/delay ${DanNetDelay} ${DanNet.Query.Received}
	/if (!${Bool[${dout}]}) /return
	/if (!${Defined[distance]}) /declare distance int local
	/if (!${Defined[tmpDistance]}) /declare tmpDistance int local
	/varset KillRetryTimer 1s
	/call GetNearest ${name}
	/if (${Macro.Return}) {
		/varset IsKillNothingUp true
		/if (${Spawn[id ${KillTargetID}].Distance3D}<20) {
			/echo killing ${KillTargetID}
			/target id ${KillTargetID}
			/stick
			/if (${Me.Pet.ID}) {
				/delay 5
				/pet attack
				/delay 5
			}
			/if (!${Me.Combat}) /attack
			/delay 1s
			/return
		}
		/if (${Spawn[id ${KillTargetID}].Distance3D}<200 && ${Target.LineOfSight} && ${pullAbility.Length}) {
			/if (${Target.ID} != ${KillTargetID}) /target id ${KillTargetID}
			
			/echo pulling LoS ${Spawn[id ${KillTargetID}].CleanName}
			/delay 5
			/casting "${pullAbility}"
			/delay 5
			/if (${Me.Pet.ID}) {
				/delay 5
				/pet attack
				/delay 5
			}
			/return
		}
	}
	/if (${KillTargetID}) {		
		/varset IsKillNothingUp true
		/if (${Navigation.Active}) /return
		/target id ${KillTargetID}
		/dquery ${name} -q "Cast.Ready[Sincere Intervention]" -o dout
		/delay ${DanNetDelay} ${DanNet.Query.Received}
		/if (${Bool[${dout}]}) {
			/dex ${name} /casting "Sincere Intervention" -targetid|${Me.ID}
			/varset KillRetryTimer 3s	
			/return
		}
		/if (${Target.Distance3D} <= 250) {
			/dquery ${name} -q "Cast.Ready[Curate's Retort]" -o dout
			/delay ${DanNetDelay} ${DanNet.Query.Received}
			/if (${Bool[${dout}]}) {
				/dex ${name} /casting "Curate's Retort" -targetid|${Me.ID}
				/varset KillRetryTimer 3s	
				/return
			}
		}
		/echo waiting on ${name} to be able to pull...
		/varset KillRetryTimer 3s
		/return
	}
	/if (${IsKillNothingUp}) /echo looks like nothing is up, pulsing every 6s
	/varset IsKillNothingUp false
/return	


sub BodyKill
	/if (${Me.Combat}) /return
	/if (${Me.Class.ShortName.Equal[SHD]} && !${Me.Invis}) /casting "Circlet of Shadow"
	/if (${Me.Casting.ID}) /return	
	/declare npcName string local
	/declare killFilter string local
	/if (!${Defined[pullAbility]}) /declare pullAbility string local
	/call PullAbility
	/varset pullAbility ${Macro.Return}
	/call RunTag
	/if (${Macro.Return}) /return
	/varset KillRetryTimer 1s
	/call GetNearest
	/if (${Macro.Return}) {
		/varset IsKillNothingUp true
		/if (${Spawn[id ${KillTargetID}].Distance3D}<20) {
			/echo killing ${KillTargetID}
			/target id ${KillTargetID}
			/stick
			/if (${Me.Pet.ID}) {
				/delay 5
				/pet attack
				/delay 5
			}
			/if (!${Me.Combat}) /attack
			/delay 1s
			/return
		}
		/if (${Target.ID} != ${KillTargetID}) {
			/target id ${KillTargetID}
			
			/echo targetting ${Spawn[id ${KillTargetID}].CleanName}
			/delay 5
			/casting "${pullAbility}"
			/delay 5
			/if (${Me.Pet.ID}) {
				/delay 5
				/pet attack
				/delay 5
			}
			/return
		}
		/return
	}
	/if (${KillTargetID}) {
		/varset IsKillNothingUp true
		/if (${Navigation.Active}) /return
		/echo going to candidate ${Spawn[id ${KillTargetID}].Distance3D}ft
		/nav id ${KillTargetID}
		/delay 1s
		/if (!${pullAbility.Length}) /return
		/if (${Spawn[id ${candidateID}].Distance3D} >= 200) /return
		/if (!${Spawn[id ${candidateID}].LineOfSight}) /return
		/if (${Target.ID} != ${KillTargetID}) {
			/target id ${KillTargetID}
			
			/echo targetting ${Spawn[id ${KillTargetID}].CleanName}
			/delay 5
			/casting "${pullAbility}"
			/delay 5
			/if (${Me.Pet.ID}) {
				/delay 5
				/pet attack
				/delay 5
			}
			/return
		}
		/return
	}
	/if (${IsKillNothingUp}) /echo looks like nothing is up, pulsing every 6s
	/varset IsKillNothingUp false
	/varset KillRetryTimer 6s
/return	

sub PullAbility
	/if (${Cast.Ready[Protest for Power Rk. II]}) /return "Protest for Power Rk. II"
	/if (${Cast.Ready[Staff of Viral Flux]}) /return "Staff of Viral Flux"
/return


sub RunTag
	/if (!${KillTargetID}) /return false
	/if (!${pullAbility.Length}) /return false	
	/if (${Spawn[id ${KillTargetID}].Distance3D} >= 200) /return false
	/if (!${Spawn[id ${KillTargetID}].LineOfSight}) /return false
	/if (${Target.ID} != ${KillTargetID}) {
		/target id ${targetID}
		
		/echo targetting ${Spawn[id ${targetID}].CleanName}
		/delay 5
		/casting "${pullAbility}"
		/delay 5
		/if (${Me.Pet.ID}) {
			/delay 5
			/pet attack
			/delay 5
		}
		/return true
	}
/return false

sub GetNearest(string clericName)
	/if (!${Defined[j]}) /declare j int local
	/if (!${Defined[k]}) /declare k int local
	/if (!${Defined[targetID]}) /declare targetID int local
	/if (!${Defined[distance]}) /declare distance int local
	/if (!${Defined[npcName]}) /declare npcName string local
	/if (!${Defined[killFilter]}) /declare killFilter string local
	/if (!${Defined[tmpDistance]}) /declare tmpDistance int local
	/if (!${Defined[candidateID]}) /declare candidateID int local
	/varset distance 1000
	/varset KillTargetID 0
	/for j 1 to ${Math.Calc[${KillTarget.Count[|]}+1]}		
		/varset npcName ${KillTarget.Token[${j},|]}
		/if (${npcName.Length}==0) /continue
		/varset killFilter npc ${npcName} zradius 100
		/if (${npcName.Equal[all]}) {
			/varset killFilter npc radius 1000 zradius 100
		}
		
		|/if (!${Me.XTarget} && ${SpawnCount[${killFilter}]}) /echo ${SpawnCount[${killFilter}]} ${npcName} are up
		/for k 1 to ${SpawnCount[${killFilter}]}
			/varset targetID ${NearestSpawn[${k}, ${killFilter}].ID}
			/if (!${IsOKToCast["${Spawn[id ${targetID}].CleanName}"]}) /continue
			/if (${Me.XTarget}) /return false
			/if (${targetID} == NULL || !${targetID}) /continue
			/if (!${Navigation.PathExists[id ${targetID}]}) /continue			
			/varset tmpDistance ${Navigation.PathLength[id ${targetID}]}
			/if (${tmpDistance} > ${distance}) /continue
			
			/varset distance ${tmpDistance}
			/varset candidateID ${targetID}

			/if (${Spawn[id ${targetID}].Distance3D} < 20) {
				/varset KillTargetID ${targetID}
				/return true
			}
			/if (!${clericName.Length} && ${Spawn[id ${targetID}].Distance3D} >= 200) /continue
			/if (!${clericName.Length} && !${Spawn[id ${targetID}].LineOfSight}) /continue
			/if (${clericName.Length} && ${Math.Distance[${Spawn[id ${targetID}].Y},${Spawn[id ${targetID}].X},${Spawn[id ${targetID}].Z}:${Spawn[pc ${clericName}].Y},${Spawn[pc ${clericName}].X}${Spawn[pc ${clericName}].Z}]} > 350) /continue
			/varset KillTargetID ${targetID}
			/return true			
		/next k
	/next j
	/if (!${isClericPull}) /varset KillTargetID ${candidateID}
/return false