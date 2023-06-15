void CMBacktracking::UpdateAnimations(C_BasePlayer *player)
{
	C_CSGOPlayerAnimState *state = player->GetPlayerAnimState();
	if (state)
	{
		// backup
		const float curtime = g_GlobalVars->curtime;
	    const float frametime = g_GlobalVars->frametime;
 
		g_GlobalVars->frametime = g_GlobalVars->interval_per_tick;		 
        g_GlobalVars->curtime = player->m_flSimulationTime();
 
		int backup_eflags = player->m_iEFlags();
 
		// SetLocalVelocity
		player->m_iEFlags() &= ~0x1000; // InvalidatePhysicsRecursive(VELOCITY_CHANGED); EFL_DIRTY_ABSVELOCITY = 0x1000
		player->m_vecAbsVelocity() = player->m_vecVelocity();
 
		// invalidates prior animations
		if (state->m_iLastClientSideAnimationUpdateFramecount() == g_GlobalVars->framecount)
			state->m_iLastClientSideAnimationUpdateFramecount() = g_GlobalVars->framecount - 1;
 
		player->m_bClientSideAnimation() = true;
 
		// updates local animations + poses + calculates new abs angle based on eyeangles and other stuff
		player->UpdateClientSideAnimation();
 
		player->m_bClientSideAnimation() = false;
 
		// restore
		player->m_iEFlags() = backup_eflags;
 
		g_GlobalVars->curtime = curtime;
		g_GlobalVars->frametime = frametime;
 
		player->InvalidateBoneCache();
		player->SetupBones(nullptr, -1, 0x7FF00, g_GlobalVars->curtime);
	}
}
Resolver( player ) ;
player->UpdateClientSideAnimation();
   void Resolver( C_CSPlayer* player ) {
	  auto animState = player->m_PlayerAnimState( );
	  auto& resolverInfo = g_ResolverData[ player->EntIndex( ) ];
	
	  Vector velocity = player->m_vecVelocity( );
	  float spd = velocity.LengthSquared( );
	  if ( spd > std::powf( 1.2f * 260.0f, 2.f ) ) {
		 Vector velocity_normalized = velocity.Normalized( );
		 velocity = velocity_normalized * ( 1.2f * 260.0f );
	  }
 
	  float v25 = Math::Clamp( player->m_flDuckAmount( ) + animState->m_fLandingDuckAdditiveSomething, 0.0f, 1.0f );
	  float v26 = animState->m_fDuckAmount;
	  float v27 = m_flChokedTime * 6.0f;
	  float v28;
 
	  // clamp
	  if ( ( v25 - v26 ) <= v27 ) {
		 if ( -v27 <= ( v25 - v26 ) )
			v28 = v25;
		 else
			v28 = v26 - v27;
	  } else {
		 v28 = v26 + v27;
	  }
 
	  float flDuckAmount = Math::Clamp( v28, 0.0f, 1.0f );
 
	  Vector animationVelocity = GetSmoothedVelocity( m_flChokedTime * 2000.0f, velocity, animState->m_vecVelocity );
	  float speed = std::fminf( animationVelocity.Length( ), 260.0f );
 
	  auto weapon = ( C_WeaponCSBaseGun* ) player->m_hActiveWeapon( ).Get( );
 
	  float flMaxMovementSpeed = 260.0f;
	  if ( weapon ) {
		 flMaxMovementSpeed = std::fmaxf( weapon->GetMaxSpeed( ), 0.001f );
	  }
 
	  float flRunningSpeed = speed / ( flMaxMovementSpeed * 0.520f );
	  float flDuckingSpeed = speed / ( flMaxMovementSpeed * 0.340f );
 
	  flRunningSpeed = Math::Clamp( flRunningSpeed, 0.0f, 1.0f );
 
	  float flYawModifier = ( ( ( animState->m_flGroundFraction * -0.3f ) - 0.2f ) * flRunningSpeed ) + 1.0f;
	  if ( flDuckAmount > 0.0f ) {
		 float flDuckingSpeed = Math::Clamp( flDuckingSpeed, 0.0f, 1.0f );
		 flYawModifier += ( flDuckAmount * flDuckingSpeed ) * ( 0.5f - flYawModifier );
	  }
 
	  float flMinBodyYaw = std::fabsf( animState->m_flMinBodyYaw * flYawModifier );
	  float flMaxBodyYaw = std::fabsf( animState->m_flMaxBodyYaw * flYawModifier ); 
}
 static auto GetSmoothedVelocity = [] ( float min_delta, Vector a, Vector b ) {
		 Vector delta = a - b;
		 float delta_length = delta.Length( );
 
		 if ( delta_length <= min_delta ) {
			Vector result;
			if ( -min_delta <= delta_length ) {
			   return a;
			} else {
			   float iradius = 1.0f / ( delta_length + FLT_EPSILON );
			   return b - ( ( delta * iradius ) * min_delta );
			}
		 } else {
			float iradius = 1.0f / ( delta_length + FLT_EPSILON );
			return b + ( ( delta * iradius ) * min_delta );
		 }
	  };
	  float flEyeYaw = player->m_angEyeAngles( ).yaw;
	  float flEyeDiff = std::remainderf( flEyeYaw - resolverInfo.m_flFakeGoalFeetYaw, 360.f );
 
	  if ( flEyeDiff <= flMaxBodyYaw ) {
		 if ( flMinBodyYaw > flEyeDiff )
			resolverInfo.m_flFakeGoalFeetYaw = fabs( flMinBodyYaw ) + flEyeYaw;
	  } else {
		 resolverInfo.m_flFakeGoalFeetYaw = flEyeYaw - fabs( flMaxBodyYaw );
	  }
 
	  resolverInfo.m_flFakeGoalFeetYaw = std::remainderf( resolverInfo.m_flFakeGoalFeetYaw, 360.f );
 
	  if ( speed > 0.1f || fabs( velocity.z ) > 100.0f ) {
		 resolverInfo.m_flFakeGoalFeetYaw = ApproachAngle(
			flEyeYaw,
			resolverInfo.m_flFakeGoalFeetYaw,
			( ( animState->m_flGroundFraction * 20.0f ) + 30.0f )
			* m_flChokedTime );
	  } else {
		 resolverInfo.m_flFakeGoalFeetYaw = ApproachAngle(
			player->GetLowerBodyYaw( ),
			resolverInfo.m_flFakeGoalFeetYaw,
			m_flChokedTime * 100.0f );
	  }
float Left = flEyeYaw + flMinBodyYaw;
float Right = flEyeYaw + flMaxBodyYaw;
float resolveYaw;
 
	  switch ( resolverInfo.m_iMissedShots % 3 ) {
		 case 0: // brute left side
		 resolveYaw = Left;
		 break;
		 case 1: // brute fake side
		 resolveYaw = resolverInfo.m_flFakeGoalFeetYaw;
		 break;
		 case 2: // brute right side
		 resolveYaw = Right;
		 break;
		 default:
		 break;
	  }
 
animState->m_flAbsRotation = resolveYaw;
void Resolver( C_CSPlayer* player ) {
	  auto animState = player->m_PlayerAnimState( );
	  auto& resolverInfo = g_ResolverData[ player->EntIndex( ) ];
	  // Rebuild setup velocity to receive flMinBodyYaw and flMaxBodyYaw
	  Vector velocity = player->m_vecVelocity( );
	  float spd = velocity.LengthSquared( );
	  if ( spd > std::powf( 1.2f * 260.0f, 2.f ) ) {
		 Vector velocity_normalized = velocity.Normalized( );
		 velocity = velocity_normalized * ( 1.2f * 260.0f );
	  }
 
	  float v25 = Math::Clamp( player->m_flDuckAmount( ) + animState->m_fLandingDuckAdditiveSomething, 0.0f, 1.0f );
	  float v26 = animState->m_fDuckAmount;
	  float v27 = m_flChokedTime * 6.0f;
	  float v28;
 
	  // clamp
	  if ( ( v25 - v26 ) <= v27 ) {
		 if ( -v27 <= ( v25 - v26 ) )
			v28 = v25;
		 else
			v28 = v26 - v27;
	  } else {
		 v28 = v26 + v27;
	  }
 
	  float flDuckAmount = Math::Clamp( v28, 0.0f, 1.0f );
 
	  Vector animationVelocity = GetSmoothedVelocity( m_flChokedTime * 2000.0f, velocity, animState->m_vecVelocity );
	  float speed = std::fminf( animationVelocity.Length( ), 260.0f );
 
	  auto weapon = ( C_WeaponCSBaseGun* ) player->m_hActiveWeapon( ).Get( );
 
	  float flMaxMovementSpeed = 260.0f;
	  if ( weapon ) {
		 flMaxMovementSpeed = std::fmaxf( weapon->GetMaxSpeed( ), 0.001f );
	  }
 
	  float flRunningSpeed = speed / ( flMaxMovementSpeed * 0.520f );
	  float flDuckingSpeed = speed / ( flMaxMovementSpeed * 0.340f );
 
	  flRunningSpeed = Math::Clamp( flRunningSpeed, 0.0f, 1.0f );
 
	  float flYawModifier = ( ( ( animState->m_flGroundFraction * -0.3f ) - 0.2f ) * flRunningSpeed ) + 1.0f;
	  if ( flDuckAmount > 0.0f ) {
		 float flDuckingSpeed = Math::Clamp( flDuckingSpeed, 0.0f, 1.0f );
		 flYawModifier += ( flDuckAmount * flDuckingSpeed ) * ( 0.5f - flYawModifier );
	  }
 
	  float flMinBodyYaw = std::fabsf( animState->m_flMinBodyYaw * flYawModifier );
	  float flMaxBodyYaw = std::fabsf( animState->m_flMaxBodyYaw * flYawModifier );
 
	  float flEyeYaw = player->m_angEyeAngles( ).yaw;
	  float flEyeDiff = std::remainderf( flEyeYaw - resolverInfo.m_flFakeGoalFeetYaw, 360.f );
 
	  if ( flEyeDiff <= flMaxBodyYaw ) {
		 if ( flMinBodyYaw > flEyeDiff )
			resolverInfo.m_flFakeGoalFeetYaw = fabs( flMinBodyYaw ) + flEyeYaw;
	  } else {
		 resolverInfo.m_flFakeGoalFeetYaw = flEyeYaw - fabs( flMaxBodyYaw );
	  }
 
	  resolverInfo.m_flFakeGoalFeetYaw = std::remainderf( resolverInfo.m_flFakeGoalFeetYaw, 360.f );
 
	  if ( speed > 0.1f || fabs( velocity.z ) > 100.0f ) {
		 resolverInfo.m_flFakeGoalFeetYaw = ApproachAngle(
			flEyeYaw,
			resolverInfo.m_flFakeGoalFeetYaw,
			( ( animState->m_flGroundFraction * 20.0f ) + 30.0f )
			* m_flChokedTime );
	  } else {
		 resolverInfo.m_flFakeGoalFeetYaw = ApproachAngle(
			player->GetLowerBodyYaw( ),
			resolverInfo.m_flFakeGoalFeetYaw,
			m_flChokedTime * 100.0f );
	  }
 
	  float Left = flEyeYaw + flMinBodyYaw;
      float Right = flEyeYaw + flMaxBodyYaw;
 
	  float resolveYaw;
 
	  switch ( resolverInfo.m_iMissedShots % 3 ) {
		 case 0: // brute left side
		 resolveYaw = Left;
		 break;
		 case 1: // brute fake side
		 resolveYaw = resolverInfo.m_flFakeGoalFeetYaw;
		 break;
		 case 2: // brute right side
		 resolveYaw = Right;
		 break;
		 default:
		 break;
	  }
 
	  animState->m_flAbsRotation = resolveYaw;
   }
