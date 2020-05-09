sub Init
	/squelch /hidecorpse alwaysnpc
	/squelch /stick set totalsilence on
	/if (!${Defined[SpellName]}) /declare SpellName string outer
	/if (!${Defined[BurnID]}) /declare BurnID int outer
	
	/if (!${Defined[IsCampEnabled]}) /declare IsCampEnabled bool outer false
	/if (!${Defined[IsEngageEnabled]}) /declare IsEngageEnabled bool outer false
	/if (!${Defined[IsFaceEnabled]}) /declare IsFaceEnabled bool outer false
	/if (!${Defined[IsForageEnabled]}) /declare IsForageEnabled bool outer false
	/if (!${Defined[KillTarget]}) /declare KillTarget string outer
	/if (!${Defined[IsMercEnabled]}) /declare IsMercEnabled bool outer false
	/if (!${Defined[IsShrinkEnabled]}) /declare IsShrinkEnabled bool outer false
	/call ${Me.Class.ShortName}Init
	/echo foo v0.0.1 for ${Me.Class} initialized, /echo help
/return