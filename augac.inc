| Engage handles stick and attack logic

#event AugAC "[MQ2] augac"

Sub Event_AugAC
	/call AugAC
/return


Sub AugAC
	/echo Scanning Type 7 Augs by AC...
	/declare cn string local
	/declare crewIndex int local
	/declare dout string local

	/declare lowestAC int local
	/declare lowestSlot string local
	|augtype
	/declare invIndex int local
	/declare augSlot int local
	/declare missingAug string local

	/for crewIndex 1 to ${Math.Calc[${DanNet.Peers.Count[|]}]}
		/varset cn ${DanNet.Peers.Token[${crewIndex},|]}
		/varset lowestAC 9999
		/varset lowestSlot none
		/varset missingAug missing: 
		/for invIndex 0 to 20
			/if (${invIndex} == 14) /continue |offhand
			/if (${invIndex} == 13) /continue |mainhand
			/if (${invIndex} == 11) /continue |ranged
			/if (${invIndex} == 21) /continue |powersource
			/if (${invIndex} == 22) /continue |ammo
			/dquery ${cn} -q "InvSlot[${invIndex}].Item.ID" -o dout
			/delay ${DanNetDelay} ${DanNet.Query.Received}
			/if (!${Int[${dout}]}) /continue

			/for augSlot 1 to 5
				/dquery ${cn} -q "InvSlot[${invIndex}].Item.AugSlot${augSlot}" -o dout
				/delay ${DanNetDelay} ${DanNet.Query.Received}
				/if (!${Int[${dout}]}) /continue
				/if (${Int[${dout}]} != 7) /continue
				/dquery ${cn} -q "InvSlot[${invIndex}].Item.Item[${augSlot}].AC" -o dout
				/delay ${DanNetDelay} ${DanNet.Query.Received}
				/if (!${Int[${dout}]}) {
					/varset missingAug missing ${InvSlot[${invIndex}].Name}
					/continue
				}
				/if (${Int[${dout}]} > ${lowestAC}) /continue
				/varset lowestAC ${Int[${dout}]}
				/varset lowestSlot ${InvSlot[${invIndex}].Name}
			/next augSlot
		/next invIndex
		/if (${lowestAC} == 9999) {
			/echo ${cn} has no augs equipped
			/continue
		}
		/if (${missingAug.Equal[missing:]}) /varset missingAug no missing augs

		/echo ${cn}'s worst: ${lowestAC}AC in ${lowestSlot}, ${missingAug}
	/next crewIndex	
	/echo Done scanning AC.
/return