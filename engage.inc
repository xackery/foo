| Engage is the core engagement logic

#event Engage "[MQ2] engage"

Sub Event_Engage
	/if (!${Defined[IsEngageEnabled]}) /declare IsEngageEnabled bool outer false
	/varcalc IsEngageEnabled (!${IsEngageEnabled})
	/echo Is engaging enabled? ${IsEngageEnabled}
/return


Sub Engage
	/if (!${Defined[IsEngageEnabled]}) /declare IsEngageEnabled bool outer false	
	/if (!${IsEngageEnabled}) /return
	/if (${Me.Song[Rallying Call VI].ID}) /return	
	/call AssistEngage
	/call MainTankEngage
	/call Kill
	/call Face
	/call Pet
/return

Sub AssistEngage
	/if (${Group.MainTank.ID} == ${Me.ID}) /return
	
	/if (${Stick.Status.Equal[ON]} && ${Navigation.Active}) {
		/stick off
		/return
	}
	
	/if (${NavTimer}) /return
	/if (${Target.ID}==0) /return
	/if (${Target.PctHPs} >= 100) /return

	/if (${Group.MainAssist.Moving}) /return
	/if (${Target.Distance}>100) /return
	/if (!${Target.LineOfSight}) /return
	/if (!${Target.Type.Equal[NPC]}) /return
	/if (${Target.Moving}) /return
	/if (${Me.Moving}) /return
	/if (${Target.Casting}) /return
		
	/if (${Math.Distance[${Group.MainAssist.Y},${Group.MainAssist.X},${Group.MainAssist.Z}:${Target.Y},${Target.X}.${Target.Z}]}>30) /return
	
	/if (!${Me.Combat}) { |&& ${IsOKToCast["${Target.CleanName}"]}) {
		/echo attacking (assistengage)
		/if (${Target.Mezzed.ID}) {
			/echo waiting to attack, mezzed
			/return
		}
		/attack on
	}

	/if (${Stick.Status.Equal[OFF]}) {
		/stick snaproll 10 moveback uw
	}
/return

sub AssistTarget
	/if (!${Defined[AssistTimer]}) /declare AssistTimer timer outer 
	/if (${AssistTimer}) /return
	/if (!${Group}) /return
	/if (${Group.MainAssist.ID} == ${Me.ID}) /return
	/if (!${Group.MainAssist.ID}) /return
	/if (!${Group.MainAssist.Present}) /return
	/if (${DanNet.Peers.Find[${EverQuest.Server}_${Group.MainAssist.Name.Lower}|]}) {
		/dquery ${Group.MainAssist.Name} -q IsAssistAllowed -t 5
		/delay 1s ${DanNet.Query.Received}
		/if (${DanNet.Query.Received} &&  ${DanNet.Query} != NULL && !${Bool[${DanNet.Query}]}) {
			/echo waiting to assist (tagging) ${DanNet.Query}
			/return
		}
	}
	
	
	/if (${AssistTimer}) /return
	/if (${Me.Moving}) /return
	/if (${Target.ID} == ${Me.GroupAssistTarget.ID}) /return
	/if (!${Me.GroupAssistTarget.LineOfSight}) /return
	/if (${Me.GroupAssistTarget.Distance}>100) /return
	/if (!${Me.GroupAssistTarget.Type.Equal[NPC]}) /return
	/if (${Me.GroupAssistTarget.Moving}) /return
	/if (${Me.GroupAssistTarget.Casting}) /return
	/if (${Group.MainAssist.Moving}) /return
	
	/echo targetting main assist target
	/if (${Me.Class.ShortName.Equal[BRD]} && ${Me.Combat}) /attack
	/target id ${Me.GroupAssistTarget.ID}
	/varset AssistTimer 3s
/return

Sub MainTankEngage
	/if (!${Defined[KillTarget]}) /declare KillTarget string outer
	
	/if (${Group.MainTank.ID} != ${Me.ID}) {
		/if (!${KillTarget.Length}) /return
	}

	
	|/if (!${Me.XTarget[1].TargetType.Equal[Auto Hater]} && ${Group.MainAssist.ID} == ${Me.ID}) {
	|	/echo setting xtar to autohater
	|	/xtar set 1 autohater
	|}

	/if (${Target.ID} && ${Stick.Active} && ${Target.Distance}>80 && !${Target.LineOfSight}) {
		/echo stopping stick, far away target
		/stick off
		/return
	}
	
	/if (${Me.Moving}) /return	
	/if (${Navigation.Active}) /return
	/if (${Target.ID}==0 || ${Target.ID}==${Me.ID}) {
		/call NearestLowHPXTarget
		/if (${Macro.Return}) {
			/echo Targetting nearest low hp during stick ${Macro.Return}
			/target id ${Macro.Return}
			/if (${Target.PctHPs}<100) /stick 10
			/return
		}
		/if (!${Me.XTarget[1].LineOfSight}) /return
		/if (${Me.XTarget[1].Distance}>40) /return
		/if (!${Me.XTarget[1].Type.Equal[NPC]}) /return
		
		/echo targetting first xtarget
		/target id ${Me.XTarget[1].ID}
	}
	
	/if (!${Target.Type.Equal[NPC]}) /return

	/if (${Target.Distance}<30 && !${Me.Combat}) {		
		/if (${IsXTarget[${Target.ID}]}) {
			/echo attacking (xtarget)
			/attack
		} else {
			/if (${Target.LineOfSight} && ${IsOKToCast["${Target.CleanName}"]}) {
				/echo attacking (oktocast)
				/attack
				/return
			}
		}
	}
	

	/if (${Stick.Active}) /return
	/if (${Me.Moving}) /return
	/if (${Target.Distance}>80) /return
	/if (!${Target.LineOfSight}) /return
	/if (!${Target.Type.Equal[NPC]}) /return
	/if (${Target.Moving}) /return
	/if (${Me.Moving}) /return

	/if (${IsTetherEnabled} && ${Math.Distance[${Target.Y},${Target.X},${Target.Z}:${CampY},${CampX}.${CampZ}]}>40) /return
	/if (${Target.PctHPs}==100) /return
	/stick 10
	/echo sticking ${Target.CleanName} ${Target.Distance}
/return