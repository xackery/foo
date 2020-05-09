| Faces an enemy more aggressively

#event Face "[MQ2] face"

Sub Event_Face
	/if (!${Defined[IsFaceEnabled]}) /declare IsFaceEnabled bool outer false
	/varcalc IsFaceEnabled (!${IsFaceEnabled})
	/echo Is facing enabled? ${IsFaceEnabled}
/return

Sub Face
	/if (!${Me.Combat}) /return
	/if (!${Defined[IsFaceEnabled]}) /declare IsFaceEnabled bool outer false
	/if (!${Defined[FaceTimer]}) /declare FaceTimer timer outer
	/if (!${IsFaceEnabled}) /return	
	/if (${FaceTimer}) /return
	/if (!${Target.ID}) {
		/varset FaceTimer 0s
		/return
	}
	/varset FaceTimer 6s
	/face fast nolook
	/echo facing ${Target.ID}
/return