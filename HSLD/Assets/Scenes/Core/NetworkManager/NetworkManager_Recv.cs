using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;

using System.Runtime.InteropServices;

using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;

using UnityEngine.Networking;
using System.Net.NetworkInformation;

public partial class NetworkManager : MonoBehaviour
{
    private bool first;

    void RecvProcess()
    {
        RecvProtocolType();
        ProcessRecvData();

        networkSyncLock = false;    // 다른 네트워크 함수의 통신을 허용합니다.
    }

    void RecvProtocolType()
    {
        try
        {
            socket.Receive(NewDataRecvBuffer);
        }
        catch (SocketException SCE)
        {
            Debug.Log("Socket connect error! : " + SCE.ToString());
            GameObject.Find("GameCores").transform.Find("AppQuitManager").GetComponent<AppQuitManager>().Quit(true, 1);
            return;
        }

        recvType = BitConverter.ToInt32(NewDataRecvBuffer, 0);

        if (recvType != (int)PROTOCOL.VOID_GAME_STATE)
            Debug.Log("RecvType is : " + recvType);
    }

    void ProcessRecvData()
    {
        if (recvType == (int)PROTOCOL.ANSWER_ANNOUNCEMENT)
        {
            int stringSizeBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);
            GameObject.Find("GameCores").transform.Find("CoreUIManager").GetComponent<CoreUIManager>().
                NetworkManager_DrawAnnouncementUI(Encoding.Unicode.GetString(NewDataRecvBuffer, 8, stringSizeBuffer));
        }
        //LoginScene
        else if (recvType == (int)PROTOCOL.FAIL_LOGIN)
        {
            GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().failReason = BitConverter.ToInt32(NewDataRecvBuffer, 4);
            GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().FailLoginProcess();
        }
        else if (recvType == (int)PROTOCOL.PERMIT_LOGIN)
        {
            winCount = BitConverter.ToInt32(NewDataRecvBuffer, 4);
            Debug.Log("win is --> " + winCount);

            loseCount = BitConverter.ToInt32(NewDataRecvBuffer, 8);
            Debug.Log("lose is --> " + loseCount);

            money = BitConverter.ToInt32(NewDataRecvBuffer, 12);
            Debug.Log("money is --> " + money);

            int achievementBit = BitConverter.ToInt32(NewDataRecvBuffer, 16);
            Debug.Log("achievementBit is --> " + achievementBit);

            itemBit = BitConverter.ToInt32(NewDataRecvBuffer, 20);
            Debug.Log("itemBit is --> " + itemBit);

            characterBit = BitConverter.ToInt32(NewDataRecvBuffer, 24);
            Debug.Log("characterBit is --> " + characterBit);

            activeCharacterIndex = BitConverter.ToInt32(NewDataRecvBuffer, 28);
            Debug.Log("activeCharacterIndex is --> " + activeCharacterIndex);

            int stringSizeBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 32);
            nickName = Encoding.Unicode.GetString(NewDataRecvBuffer, 36, stringSizeBuffer);
            Debug.Log("nickName is --> " + nickName);
            //
            if (money == -1)
                GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().UI_OnOff_NewNicknameUI();
            else
                GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().PermitLoginProcess();
        }
        else if (recvType == (int)PROTOCOL.PERMIT_NICKNAME)
        {
            isFirstLogin = true;

            if (BitConverter.ToBoolean(NewDataRecvBuffer, 4))
                GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().PermitLoginProcess();
            else
                GameObject.Find("SignUp_UI").transform.Find("Text_State").transform.Find("Text").gameObject.GetComponent<Text>().text = "이미 사용중인 닉네임입니다.";
        }

        //MainUIScene
        else if (recvType == (int)PROTOCOL.PERMIT_FRIEND_INFO)
        {
            /*
             서버로 부터, 친구들 정보를 받고 UI 갱신 함수를 호출합니다.
             */

            /* 변수 초기화 처리 */
            for (int i = 0; i < 4; ++i)
            {
                //friendIdCont[i] = " ";
                //friendNickNameCont[i] = " ";
                friendState[i] = 0;
            }

            /* 데이터 송수신 처리 */
            friendNum = BitConverter.ToInt32(NewDataRecvBuffer, 4);
            Debug.Log("친구 수는 : " + friendNum);

            int dataLocation = 8;
            int idSize;
            int iBuffer;

            for (int i = 0; i < friendNum; ++i)
            {
                //idSize = BitConverter.ToInt32(NewDataRecvBuffer, dataLocation);
                //friendNickNameCont[i] = Encoding.Default.GetString(NewDataRecvBuffer, dataLocation + 4, idSize);
                //dataLocation += idSize + 8;


                iBuffer = BitConverter.ToInt32(NewDataRecvBuffer, dataLocation);
                friendState[i] = iBuffer;

                friendActiveCharacterIndex[i] = (int)(NewDataRecvBuffer[dataLocation + 4]);//(int)(BitConverter.ToChar(NewDataRecvBuffer, dataLocation + 4));

                if (iBuffer == 0)
                {
                    //Not Login!
                    // 친구 닉네임이 갑자기 없어짐... 이런경우 있으면 안됨!!
                    Debug.Log("에혀...친구닉네임이 없어질리가 없는데 ???????????????????");

                    //friendState[i] = 0;
                    dataLocation += 5;
                    continue;
                }
                else
                {
                    idSize = BitConverter.ToInt32(NewDataRecvBuffer, dataLocation + 5);

                    //friendNickNameCont[i] = Encoding.Default.GetString(NewDataRecvBuffer, dataLocation + 8, idSize);    //DEV_66
                    friendNickNameCont[i] = Encoding.Unicode.GetString(NewDataRecvBuffer, dataLocation + 9, idSize);    //DEV_66

                    dataLocation += idSize + 9;

                    Debug.Log("캐릭터 :" + friendActiveCharacterIndex[i].ToString() + "상태는 : " + iBuffer + "닉네임 크기 : " + idSize + " 닉네임 : " + friendNickNameCont[i]);

                    //조건문 전 처리.
                    //if (iBuffer == 1)
                    //    friendState[i] = 1; // 로그인 안함.
                    //else if (iBuffer == 2)
                    //    friendState[i] = 2; // 로그인 했고 초대 가능상태.
                    //else if (iBuffer == 3)
                    //    friendState[i] = 3; // 로그인 했고 현재 게임중인상태.
                }
            }

            // 친구 UI On
            GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().OnFriendUI_NetworkManager(friendNum);
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_FRIEND_INVITE)
        {
            /*
                DemandFriendInvite에 대한 결과값이 옴. 
                4~7이 true이면 가능으로 상대 응답 대기상태, false이면 불가능 UI 띄움.  
            */

            // 초대한 친구만 받는 프로토콜, 1이면 친구가 정상적, 0이면 친구상태가 나갔거나 게임중으로 변경.
            int inviteBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            if (inviteBuffer == 1)
            {
                isHost = true;
                GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().OnFriendWaitUI_NetworkManager();
                //친구가 정상적인 상태
            }
            else if (inviteBuffer == 0)
            {
                GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().OnFriendBadStateUI_NetworkManager(1);
                // 친구가 비정상적인 상태
            }
            //GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>()
        }
        else if (recvType == (int)PROTOCOL.GUESTCHECK_FRIEND_INVITE)
        {
            // 초대받은 친구(게스트)가 받는 프로트콜, [ False면, 방 삭제 ]  [ True면, 방접속된 상태]
            // 2일 경우,,,,, 내가 찬성을 눌렀는데도 방이 없어진 상태 

            int inviteBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            if (inviteBuffer == 1)
            {
                isHost = false;

                //친구가 정상적인 상태
                if (BitConverter.ToInt32(NewDataRecvBuffer, 8) == 1)
                {
                    if (isHost == true)
                    {
                        isAttackFirst = true;
                    }
                    else
                    {
                        isAttackFirst = false;
                    }
                }
                else
                {
                    if (isHost == true)
                    {
                        isAttackFirst = false;
                    }
                    else
                    {
                        isAttackFirst = true;
                    }
                }

                playerMissionIndex = BitConverter.ToInt32(NewDataRecvBuffer, 12);
                enemyMissionIndex = BitConverter.ToInt32(NewDataRecvBuffer, 16);
                subMissionIndex = BitConverter.ToInt32(NewDataRecvBuffer, 20);

                int indexBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 24);
                enemyId = friendNickNameCont[indexBuffer];

                Debug.Log("적 닉네임은 : " + enemyId + "이다.");

                // DEV_77
                //GameObject.Find("GameCores").transform.Find("UserDataUI").gameObject.SetActive(false);

                //GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().ChangeRoomScene();
                GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.ROOM_SCENE, true, LOAD_UI_TYPE.BABY_CHICKEN, 3.0f);
            }
            else if (inviteBuffer == 0)
            {
                // 할거 없는 상태
            }
            else if (inviteBuffer == 2)
            {
                // 팝업 하나 추가
            }
        }
        else if (recvType == (int)PROTOCOL.HOSTCHECK_FRIEND_INVITE)
        {
            /*
                호스트 플레이어가, UI를 키고, 게스트 플레이어(친구)를 기다릴 때,
                상대방이 거절하거나, 연락두절일때! ( 수락했을 때는, ANSWER_FRIEND_JOIN 을 서버에서 보내줌)
             */

            // 0이면 상대방 거절, 1이면 미입력 or UDP소실
            int inviteBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            if (inviteBuffer == 0)
            {
                GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().OffFriendWaitUI_NetworkManager();
            }
            else if (inviteBuffer == 1)
            {
                // 이 때는, 7초마다 하는게 아니고, 호스트의 마지막 요청에 대해서 여전히 무응답일 경우 여기에 걸림.
                GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().OffFriendWaitUI_NetworkManager();
            }
        }

        else if (recvType == (int)PROTOCOL.CHECK_DEMAND_MAKE_FRIEND)
        {
            int iBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            if (iBuffer == 1)
            {
                // 해줄 부분 없음? -> 상대방에게 친구 요청을 정상적으로 보냈습니다.
                GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().OnUI_CHECK_DEMAND_MAKE_FRIEND(-1);
            }
            else
            {
                iBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 8);

                //if(iBuffer == 0)
                //else if (iBuffer == 1)
                //else if (iBuffer == 2)
                GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().OnUI_CHECK_DEMAND_MAKE_FRIEND(iBuffer);
            }
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_MAKE_FRIEND_INFO)
        {
            int iBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);
            string nickNameBuffer = Encoding.Unicode.GetString(NewDataRecvBuffer, 8, iBuffer);

            //Debug.Log("받은 닉네임은 : " + nickNameBuffer);
            GameObject.Find("GameCores").transform.Find("CoreUIManager").GetComponent<CoreUIManager>().OnUI_DEMAND_FRIEND_UDP(nickNameBuffer);
        }
        else if (recvType == (int)PROTOCOL.CHECK_ANSWER_MAKE_FRIEND)
        {
            int iBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            GameObject.Find("GameCores").transform.Find("CoreUIManager").GetComponent<CoreUIManager>().OnUI_CHECK_ANSWER_MAKE_FRIEND(iBuffer);
        }

        else if (recvType == (int)PROTOCOL.ANSWER_BUY_ITEM)
        {
            int iBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);
            money = BitConverter.ToInt32(NewDataRecvBuffer, 8);

            GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().
            NetworkManager_AnswerBuy(iBuffer);
        }

        else if (recvType == (int)PROTOCOL.ANSWER_VIP_CODE)
        {
            int iBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            if (iBuffer == -1)
            {
                GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().NetworkManager_RecvVipResult(true);
            }
            else if (iBuffer == 0)
            {
                GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().NetworkManager_RecvVipResult(false, false);
            }
            else if (iBuffer == 1)
            {
                GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().NetworkManager_RecvVipResult(false, true);
            }
        }

        else if (recvType == (int)PROTOCOL.ANSWER_BUY_CHARACTER)
        {
            int typeBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().
            NetworkManager_AnswerBuyCharacter(typeBuffer);
        }

        else if (recvType == (int)PROTOCOL.PERMIT_ACTIVE_CHARACTER)
        {
            // 여기서 원래 아무짓도 안해줘도 됌!
            return;
        }

        //else if (recvType == (int)PROTOCOL.ANSWER_MAKE_FRIEND)
        //{
        //    // 1이면 정상적으로 방삭제된 상태, 0이면 게스트 그사이접속해버렷네?
        //    int iBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);
        //
        //    if (iBuffer == 1)
        //    {
        //        // 여기서 갱신해줘야하는데 귀찮으니까 고냥 창을 꺼버릴까? 오 왕쩌는걸?
        //        // 아니지 고냥 여기서 또 보내주면 돼지!....ㅎ
        //        recvType = 0;
        //
        //        GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().OffFriendUI();
        //        SendData((int)PROTOCOL.DEMAND_FRIEND_INFO);
        //    }
        //    else
        //    {
        //        // 여기서 할거 없고.
        //    }
        //}

        //LobbyScene - old
        //else if (recvType == (int)PROTOCOL.PERMIT_MAKEROOM)
        //else if (recvType == (int)PROTOCOL.PERMIT_JOINROOM)
        //else if (recvType == (int)PROTOCOL.FAIL_JOINROOM)

        //LobbyScene - new
        else if (recvType == (int)PROTOCOL.PERMIT_MAKE_RANDOM)
        {
            isHost = true;
            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().isRecvTrue = true;

            if (BitConverter.ToInt32(NewDataRecvBuffer, 4) == 1)
            {
                if (isHost == true)
                {
                    isAttackFirst = true;
                }
                else
                {
                    isAttackFirst = false;
                }
            }
            else
            {
                if (isHost == true)
                {
                    isAttackFirst = false;
                }
                else
                {
                    isAttackFirst = true;
                }
            }

            playerMissionIndex = BitConverter.ToInt32(NewDataRecvBuffer, 8);
            enemyMissionIndex = BitConverter.ToInt32(NewDataRecvBuffer, 12);
            subMissionIndex = BitConverter.ToInt32(NewDataRecvBuffer, 16);
        }
        else if (recvType == (int)PROTOCOL.PERMIT_JOIN_RANDOM)
        {
            isHost = false;
            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().isRecvTrue = true;

            //int isHostFirst;
            if (BitConverter.ToInt32(NewDataRecvBuffer, 4) == 1)
            {
                if (isHost == true)
                {
                    isAttackFirst = true;
                }
                else
                {
                    isAttackFirst = false;
                }
            }
            else
            {
                if (isHost == true)
                {
                    isAttackFirst = false;
                }
                else
                {
                    isAttackFirst = true;
                }
            }

            playerMissionIndex = BitConverter.ToInt32(NewDataRecvBuffer, 8);
            enemyMissionIndex = BitConverter.ToInt32(NewDataRecvBuffer, 12);
            subMissionIndex = BitConverter.ToInt32(NewDataRecvBuffer, 16);
            enemyCharacterIndex = BitConverter.ToInt32(NewDataRecvBuffer, 20);

            int idSizeBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 24);
            enemyId = Encoding.Unicode.GetString(NewDataRecvBuffer, 28, idSizeBuffer);
            Debug.Log("적 닉네임은 : " + enemyId);

            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().ChangeRoomScene();
        }
        else if (recvType == (int)PROTOCOL.ANSWER_EXIT_RANDOM)
        {
            isHost = false; // 굳이 상관은 없어보임.
            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().SuccessExitMatching();
        }

        else if (recvType == (int)PROTOCOL.ANSWER_FRIEND_JOIN)
        {
            /*
                초대한 친구(호스트)가, 7초 전까지 매초마다 친구들어왔는 지 확인하는 함수. 하 내가 이걸 왜 로비로 뺏을 까
            */

            int inviteBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            if (inviteBuffer == 0)
            {
                // 고냥 아직 친구 안들어온거임.
            }
            else
            {
                isHost = true;

                if (BitConverter.ToInt32(NewDataRecvBuffer, 8) == 1)
                {
                    if (isHost == true)
                    {
                        isAttackFirst = true;
                    }
                    else
                    {
                        isAttackFirst = false;
                    }
                }
                else
                {
                    if (isHost == true)
                    {
                        isAttackFirst = false;
                    }
                    else
                    {
                        isAttackFirst = true;
                    }
                }

                playerMissionIndex = BitConverter.ToInt32(NewDataRecvBuffer, 12);
                enemyMissionIndex = BitConverter.ToInt32(NewDataRecvBuffer, 16);
                subMissionIndex = BitConverter.ToInt32(NewDataRecvBuffer, 20);

                //게임시작 개굿 ( UserDataUI가 On이면 꺼줘야함)
                // DEV_77
                //GameObject.Find("GameCores").transform.Find("UserDataUI").gameObject.SetActive(false);

                //GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().ChangeRoomScene();
                GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.ROOM_SCENE, true, LOAD_UI_TYPE.BABY_CHICKEN, 2.0f);
            }
        }

        // RoomScene
        else if (recvType == (int)PROTOCOL.PERMIT_GUEST_JOIN)
        {
            enemyCharacterIndex = BitConverter.ToInt32(NewDataRecvBuffer, 4);
            int idSizeBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 8);
            enemyId = Encoding.Unicode.GetString(NewDataRecvBuffer, 12, idSizeBuffer);

            Debug.Log("적닉네임입니다. 크기 :" + idSizeBuffer + "ID : " + enemyId);
            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().ChangeRoomScene();
        }
        else if (recvType == (int)PROTOCOL.PERMIT_GUEST_NOT_JOIN)
        {
            //아모고토못하죠
        }

        //RoomScene - old
        //else if (recvType == (int)PROTOCOL.ROOMSTATE_VOID)
        //else if (recvType == (int)PROTOCOL.ROOMSTATE_GUESTIN)

        //RoomScene - new
        else if (recvType == (int)PROTOCOL.PERMIT_ENEMY_CHARACTER)
        {
            enemyCharacterIndex = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            GameObject.Find("RoomSceneManager").GetComponent<RoomSceneManager>().SetEnemyCharacter_Network();
        }

        //InGameScene //-> 이부분 다 InGameScene으로 들어내도 괜찮을듯!
        else if (recvType == (int)PROTOCOL.VOID_GAME_STATE)
        {
            // 뭐야 니 아무것도 없엉 // ? 생김.

            if (!inGameSceneManager.isOnWaitGameReady)
            {
                inGameSceneManager.StartInGameCoroutine();
                Debug.Log(" (int)PROTOCOL.VOID_GAME_STATE 이 불림. ");
            }
            //Debug.Log(" (int)PROTOCOL.VOID_GAME_STATE 이 불렸는 가? ");
            //return; //recvProtocolFlag 안쓸것 같긴 한데, 할튼 일단 꺼졍.
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_CHANGE_TURN)
        {
            inGameSceneManager.RecvChangeTurn(BitConverter.ToInt32(NewDataRecvBuffer, 4));
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_DICE_VALUE)
        {
            //inGameSceneManager.network_terrainType = BitConverter.ToInt32(DataRecvBuffer, 4);
            // inGameSceneManager.network_changeTerrainCount = BitConverter.ToInt32(DataRecvBuffer, 8);

            // for (int i = 0; i < inGameSceneManager.network_changeTerrainCount; ++i)
            // {
            //     inGameSceneManager.network_terrainIndex[i] = BitConverter.ToInt32(DataRecvBuffer, 12 + 4 * i);
            // }

            inGameSceneManager.RecvDiceValue(BitConverter.ToInt32(NewDataRecvBuffer, 4));
            Debug.Log("받은 diceValueForLoop 값은 : " + inGameSceneManager.diceValueForLoop);
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_TERRAIN_TYPE) 
        {
            //inGameSceneManager.network_eventCardType = BitConverter.ToInt32(DataRecvBuffer, 4);
            // inGameSceneManager.network_terrainType = BitConverter.ToInt32(DataRecvBuffer, 8);
            //inGameSceneManager.network_changeTerrainCount = BitConverter.ToInt32(DataRecvBuffer, 12);

            //for (int i = 0; i < inGameSceneManager.network_changeTerrainCount; ++i)
            // {
            //     inGameSceneManager.network_terrainIndex[i] = BitConverter.ToInt32(DataRecvBuffer, 16 + 4 * i);
            // }
            inGameSceneManager.RecvTerrainType(BitConverter.ToInt32(NewDataRecvBuffer, 4));
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_TERRAIN_INDEXS)
        {
            if (first == false)
            {
                GameObject.FindWithTag("GameManager").GetComponent<InGameSceneManager>().diceValueForLoop++;
                first = true;
            }
            //inGameSceneManager.network_eventCardType = BitConverter.ToInt32(DataRecvBuffer, 4);
            int arrSizeBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);
            Debug.Log(" 총 받아야하는 배열의 크기는 " + arrSizeBuffer + "입니다");

            for (int i = 0; i < inGameSceneManager.diceValueForLoop; i++)
            {
                Debug.Log(" " + i + " 번 까지는 정상적으로 넣었습니다. " + (8 + (4 * i)) + "의 부터 형변환 한 값 ::" + BitConverter.ToInt32(NewDataRecvBuffer, (8 + (4 * i))));
                inGameSceneManager.recvTerrainIndex[i] = BitConverter.ToInt32(NewDataRecvBuffer, (8 + (4 * i)));
            }

            Debug.Log("받은 diceValueForLoop 값은 : " + inGameSceneManager.diceValueForLoop);
            inGameSceneManager.RecvTerrainIndex();
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_EVENTCARD_INDEX)
        {
            inGameSceneManager.NetworkManager_RecvEventcardIndex(BitConverter.ToInt32(NewDataRecvBuffer, 4));
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_EMOJI)
        {
            inGameSceneManager.NetworkManager_TurnOnEnemyEmoji(BitConverter.ToInt32(NewDataRecvBuffer, 4));
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_GAME_END)
        {
            inGameSceneManager.RecvGameEnd(false);
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_GAME_BUFFER)
        {
            inGameSceneManager.RecvGameEnd(true);
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_EVENT_BUFFER)
        {
            inGameSceneManager.NetworkManager_RecvEventBuffer(BitConverter.ToInt32(NewDataRecvBuffer, 4));
        }
        // Network Exception
        else if (recvType == (int)PROTOCOL.DISCONNECTED_ENEMY_CLIENT)
        {
            //
            // BGM 제가 꺼드리겠습니다.
            //GameObject.Find("GameCores").transform.Find("SoundManager").GetComponent<SoundManager>().BGMMaximumVolume = 0;

            // 씬 이동 ( 추후 상대방 탕출에 의한 승리 결과창 및, 씐 이동으로 변경 필요)
            GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.MainUI_SCENE, true, LOAD_UI_TYPE.BABY_CHICKEN, 3.0f);
            //
            //SendData((int)PROTOCOL.DOUBLECHECK_DISCONNECTED_ENEMY_CLIENT);
        }

        recvType = 0;
    }
}
