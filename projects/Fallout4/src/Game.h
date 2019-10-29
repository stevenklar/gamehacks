#pragma once
#include <cstdint>

class EquippedWeaponData
{
public:
	char pad_0000[24]; //0x0000
	int64_t ammo; //0x0018
	class WeaponProperties* pProperties; //0x0020
	char pad_0028[88]; //0x0028
}; //Size: 0x0080

class SomeStats
{
public:
	char pad_0000[8]; //0x0000
	float value; //0x0008
	char pad_000C[5484]; //0x000C
}; //Size: 0x1578

class WeaponProperties
{
public:
	float noSpreadMin; //0x0000
	float noSpreadMax; //0x0004
	char pad_0008[24]; //0x0008
	float noRecoilMin; //0x0020
	float noRecoilMax; //0x0024
	char pad_0028[88]; //0x0028
}; //Size: 0x0080

class N0000034F
{
public:
	class SomeStats* N00000350; //0x0000
	char pad_0008[128]; //0x0008
}; //Size: 0x0088

class N000006DB
{
public:
	char pad_0000[128]; //0x0000
	class N000007C8* N000006EB; //0x0080
}; //Size: 0x0088

class ActorValueInfo
{
public:
	char pad_0000[184]; //0x0000
	float yaw; //0x00B8
	float N000013EF; //0x00BC
	float pitch; //0x00C0
	float N000013F1; //0x00C4
	float N00000987; //0x00C8
	float N000013F5; //0x00CC
	float N00000988; //0x00D0
	float N000013F7; //0x00D4
	char pad_00D8[552]; //0x00D8
	class ActorMover* pActorMover; //0x0300
	char pad_0308[316]; //0x0308
	float m_HP; //0x0444
	char pad_0448[8]; //0x0448
	float m_AP; //0x0450
	char pad_0454[20]; //0x0454
	float m_RAD; //0x0468
	float m_Timer; //0x046C
	char pad_0470[40]; //0x0470
}; //Size: 0x0498

class N000007C8
{
public:
	char pad_0000[1160]; //0x0000
}; //Size: 0x0488

class hknpBSCharacterProxy
{
public:
	char pad_0000[32]; //0x0000
	class hknpDynamicCompoundShape* pHknpDynamicCompoundShape; //0x0020
	char pad_0028[16]; //0x0028
	class hknpBSWorld* pHknpBSWorld; //0x0038
	char pad_0040[48]; //0x0040
	float m_PosX; //0x0070
	float m_PosY; //0x0074
	float m_PosZ; //0x0078
	char pad_007C[132]; //0x007C
	class N0000064D* N000004FA; //0x0100
	char pad_0108[40]; //0x0108
	class bhkCharacterProxy* pBhkCharacterProxy; //0x0130
	char pad_0138[264]; //0x0138
}; //Size: 0x0240

class hknpDynamicCompoundShape
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088

class hknpBSWorld
{
public:
	char pad_0000[1160]; //0x0000
}; //Size: 0x0488

class N0000064D
{
public:
	class bhkCharProxyController* pBhkCharProxyController; //0x0000
	char pad_0008[128]; //0x0008
}; //Size: 0x0088

class bhkCharacterProxy
{
public:
	char pad_0000[1112]; //0x0000
	class N0000626F* N000061E9; //0x0458
	char pad_0460[1056]; //0x0460
}; //Size: 0x0880

class bhkCharProxyController
{
public:
	char pad_0000[32]; //0x0000
	class BSFadeNode* pBSFadeNode; //0x0020
	char pad_0028[8]; //0x0028
	class bhkPhysicsSystem* pBhkPhysicsSystem; //0x0030
	char pad_0038[80]; //0x0038
}; //Size: 0x0088

class BSFadeNode
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088

class bhkPhysicsSystem
{
public:
	char pad_0000[392]; //0x0000
}; //Size: 0x0188

class ppActor
{
public:
	class Actor* pActor; //0x0000
	char pad_0008[120]; //0x0008
}; //Size: 0x0080

class PlayerPtr
{
public:
	class bhkCharProxyController2* pCharProxy; //0x0000
	char pad_0008[120]; //0x0008
}; //Size: 0x0080

class bhkCharProxyController2
{
public:
	char pad_0000[32]; //0x0000
	class BSFadeNode2* pFadeNode; //0x0020
	char pad_0028[1112]; //0x0028
	class hknpBSCharacterProxy* pCharProxy; //0x0480
	char pad_0488[1088]; //0x0488
	class hknpCharacterProxyListener* pProxyListener1; //0x08C8
	char pad_08D0[8]; //0x08D0
	class hknpCharacterProxyListener* pProxyListener2; //0x08D8
	char pad_08E0[8]; //0x08E0
	class hknpCharacterProxyListener* pProxyListener3; //0x08E8
	char pad_08F0[80]; //0x08F0
}; //Size: 0x0940

class pActorPtr
{
public:
	class Actor* pActor; //0x0000
	char pad_0008[1144]; //0x0008
}; //Size: 0x0480

class Actor
{
public:
	char pad_0000[8]; //0x0000
	class ActorValueInfo pActorValueInfo; //0x0008
}; //Size: 0x04A0

class N00000B3F
{
public:
	char pad_0000[1136]; //0x0000
	class hknpBSCharacterProxy* N00000BCE; //0x0470
	char pad_0478[1040]; //0x0478
}; //Size: 0x0888

class N00000CB2
{
public:
	char pad_0000[1144]; //0x0000
	float AP; //0x0478
	char pad_047C[12]; //0x047C
}; //Size: 0x0488

class ActorMover
{
public:
	char pad_0000[8]; //0x0000
	class ActorValueInfo* N00000D67; //0x0008
	char pad_0010[120]; //0x0010
}; //Size: 0x0088

class N00000D94
{
public:
	char pad_0000[1112]; //0x0000
	float AP; //0x0458
	char pad_045C[1068]; //0x045C
}; //Size: 0x0888

class BSFadeNode2
{
public:
	char pad_0000[272]; //0x0000
	class GlobalPlayer* pGlobalPlayer; //0x0110
}; //Size: 0x0118

class GlobalPlayer
{
public:
	char pad_0000[1100]; //0x0000
	float m_TP; //0x044C
	char pad_0450[8]; //0x0450
	float m_AP; //0x0458
	char pad_045C[20]; //0x045C
	float m_RAD; //0x0470
	char pad_0474[2068]; //0x0474
}; //Size: 0x0C88

class N00000E15
{
public:
	char pad_0000[216]; //0x0000
	class pActorPtr2* N00000EB0; //0x00D8
	char pad_00E0[136]; //0x00E0
}; //Size: 0x0168

class pActorPtr2
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088

class xExtraPersistentCell
{
public:
	char pad_0000[8]; //0x0000
	class xExtraLevCreaModifier* N00000ECA; //0x0008
	char pad_0010[16]; //0x0010
}; //Size: 0x0020

class xExtraLevCreaModifier
{
public:
	char pad_0000[8]; //0x0000
	class xExtraLinkedRef* N00000EDB; //0x0008
	char pad_0010[80]; //0x0010
}; //Size: 0x0060

class xExtraLinkedRef
{
public:
	char pad_0000[8]; //0x0000
	class xExtraActivateRef* N00000EEC; //0x0008
	char pad_0010[112]; //0x0010
}; //Size: 0x0080

class xExtraActivateRef
{
public:
	char pad_0000[8]; //0x0000
	class xExtraLeveledCreature* N00000EFD; //0x0008
	char pad_0010[112]; //0x0010
}; //Size: 0x0080

class xExtraLeveledCreature
{
public:
	char pad_0000[8]; //0x0000
	class xBGSObjectInstanceExtra* N00000F0E; //0x0008
	char pad_0010[112]; //0x0010
}; //Size: 0x0080

class xBGSObjectInstanceExtra
{
public:
	char pad_0000[8]; //0x0000
	class xExtraInstanceData* N00000F1F; //0x0008
	char pad_0010[112]; //0x0010
}; //Size: 0x0080

class xExtraInstanceData
{
public:
	char pad_0000[8]; //0x0000
	class xExtraPromotedRef* N00000F30; //0x0008
	char pad_0010[16]; //0x0010
	class TESNPC_InstanceData* pInstanceData; //0x0020
	char pad_0028[80]; //0x0028
}; //Size: 0x0078

class xExtraPromotedRef
{
public:
	char pad_0000[48]; //0x0000
}; //Size: 0x0030

class TESNPC_InstanceData
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088

class ActorValueInfoVTable
{
public:
	char pad_0008[3712]; //0x0008

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
}; //Size: 0x0E88

class N000014E2
{
public:
	char pad_0000[2120]; //0x0000
	int8_t N000014EC; //0x0848
	char pad_0849[55]; //0x0849
}; //Size: 0x0880

class N00004E6B
{
public:
	char pad_0000[96]; //0x0000
	class N00004E84* N00004E78; //0x0060
	char pad_0068[32]; //0x0068
}; //Size: 0x0088

class N00004E84
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088

class N00004F9B
{
public:
	char pad_0000[296]; //0x0000
	class N0000526D* N00005030; //0x0128
	char pad_0130[1856]; //0x0130
}; //Size: 0x0870

class N0000526D
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088

class N0000626F
{
public:
	char pad_0000[2184]; //0x0000
}; //Size: 0x0888

class N0000667C
{
public:
	char pad_0000[8]; //0x0000
	class bhkCharProxyController2* N0000667E; //0x0008
	char pad_0010[40]; //0x0010
	class bhkCharProxyController2* N00006684; //0x0038
	char pad_0040[4160]; //0x0040
}; //Size: 0x1080

class N00006A08
{
public:
	class N00006A1B* N00006A09; //0x0000
	char pad_0008[120]; //0x0008
}; //Size: 0x0080

class N00006A1B
{
public:
	char pad_0000[120]; //0x0000
	class bhkCharProxyController2* N00006A2D; //0x0078
	char pad_0080[4104]; //0x0080
}; //Size: 0x1088

class pCharacterProxyControllerList
{
public:
	class bhkCharProxyController2* aCharaterProxyControllerList[20]; //0x0000
	int32_t entityCount; //0x00A0
}; //Size: 0x00A4

class N00006DD6
{
public:
	char pad_0000[24]; //0x0000
	class N00006DEA* N00006DDA; //0x0018
	char pad_0020[104]; //0x0020
}; //Size: 0x0088

class N00006DEA
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088

class hknpCharacterProxyListener
{
public:
	char pad_0000[1040]; //0x0000
	class Actor* pActor; //0x0410
}; //Size: 0x0418
