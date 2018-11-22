using System.Collections;
using System.Collections.Generic;
using UnityEngine;

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

public partial class NetworkManager : MonoBehaviour {
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
        //LoginScene
        if (recvType == (int)PROTOCOL.FAIL_LOGIN)
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

            int titleBit = BitConverter.ToInt32(NewDataRecvBuffer, 20);
            Debug.Log("titleBit is --> " + titleBit);

            int characterBit = BitConverter.ToInt32(NewDataRecvBuffer, 24);
            Debug.Log("characterBit is --> " + characterBit);

            int stringSizeBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 28);
            nickName = Encoding.Default.GetString(NewDataRecvBuffer, 32, stringSizeBuffer);
            Debug.Log("nickName is --> " + nickName);
            //
            if (money == -1)
                GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().OnNewNicknameUI();
            else
                GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().PermitLoginProcess();
        }
        else if (recvType == (int)PROTOCOL.PERMIT_NICKNAME)
        {
            GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().PermitLoginProcess();
        }

        //MainUIScene
        else if (recvType == (int)PROTOCOL.PERMIT_FRIEND_INFO)
        {
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

                if (iBuffer == 0)
                {
                    //Not Login!

                    //friendState[i] = 0;
                    dataLocation += 4;
                    continue;
                }
                else
                {
                    idSize = BitConverter.ToInt32(NewDataRecvBuffer, dataLocation + 4);
                    friendNickNameCont[i] = Encoding.Default.GetString(NewDataRecvBuffer, dataLocation + 8, idSize);

                    dataLocation += idSize + 12;

                    iBuffer = BitConverter.ToInt32(NewDataRecvBuffer, dataLocation - 4);

                    Debug.Log("상태는 : " + iBuffer + "닉네임 크기 : " + idSize + " 닉네임 : " + friendNickNameCont[i]);
                    if (iBuffer == 0)
                    {
                        // 로그인 했지만, 인 게임 불가능
                        friendState[i] = 1;
                    }
                    else
                    {
                        // 로그인 했고 초대 가능상태.
                        friendState[i] = 2;
                    }
                }
            }

            // 친구 UI On
            GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().OnFriendUI_NetworkManager(friendNum);
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_FRIEND_INVITE)
        {
            // 초대한 친구만 받는 프로토콜, 1이면 친구가 정상적, 0이면 친구상태가 나갔거나 게임중으로 변경.
            int inviteBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            if (inviteBuffer == 1)
            {
                isHost = true;
                GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().OnFriendWaitUI_NetworkManager();
                //친구가 정상적인 상태
            }
            else
            {
                GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().OnFriendBadStateUI_NetworkManager(1);
                // 친구가 비정상적인 상태
            }
            //GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>()
        }
        else if (recvType == (int)PROTOCOL.GUESTCHECK_FRIEND_INVITE)
        {
            // 초대받은 친구만 받는 프로트콜, False면, 방 삭제 True면, 방접속된 상태.

            int inviteBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            if (inviteBuffer == 1)
            {
                //친구가 정상적인 상태
            }
            else
            {
                // 친구가 비정상적인 상태
            }

        }
        else if (recvType == (int)PROTOCOL.HOSTCHECK_FRIEND_INVITE)
        {
            // 
            //  0이면 안들어옴. 1이면 들어옴. 2이면 방삭제함.
            int inviteBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            if (inviteBuffer == 0)
            {
                //? 아무것도 안해도됨.
            }
            else if (inviteBuffer == 1)
            {
                // 게임 방 정보 요구하고, 룸씬으로 변경.

            }
            else if (inviteBuffer == 2)
            {
                // 상대방이 초대 안받은거 알림.
            }
        }

        else if (recvType == (int)PROTOCOL.CHECK_DEMAND_MAKE_FRIEND)
        {
            int iBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            if(iBuffer == 1)
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
            string nickNameBuffer = Encoding.Default.GetString(NewDataRecvBuffer, 8, iBuffer);

            GameObject.Find("GameCores").transform.Find("CoreUIManager").GetComponent<CoreUIManager>().OnUI_DEMAND_FRIEND_UDP(nickNameBuffer);
        }
        else if (recvType == (int)PROTOCOL.CHECK_ANSWER_MAKE_FRIEND)
        {
            int iBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            GameObject.Find("GameCores").transform.Find("CoreUIManager").GetComponent<CoreUIManager>().OnUI_CHECK_ANSWER_MAKE_FRIEND(iBuffer);
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

            int idSizeBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 20);
            enemyId = Encoding.Default.GetString(NewDataRecvBuffer, 24, idSizeBuffer);
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
            // 초대한 친구가, 7초 전까지 매초마다 친구들어왔는 지 확인. 하 내가 이걸 왜 로비로 뺏을 까

            // 1이면 정상적으로 방삭제된 상태, 0이면 게스트 그사이접속해버렷네?
            int inviteBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);

            if (inviteBuffer == 1)
            {
                isHost = false; // 굳이 상관은 없어보임.
                //GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().SuccessExitMatching();
            }
            else
            {
                int idSizeBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 8);
                enemyId = Encoding.Default.GetString(NewDataRecvBuffer, 12, idSizeBuffer);

                //게임시작 개굿
            }
        }

        // RoomScene
        else if (recvType == (int)PROTOCOL.PERMIT_GUEST_JOIN)
        {
            int idSizeBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);
            enemyId = Encoding.Default.GetString(NewDataRecvBuffer, 8, idSizeBuffer);

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
            }
            //return; //recvProtocolFlag 안쓸것 같긴 한데, 할튼 일단 꺼졍.
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_CHANGE_TURN)
        {
            inGameSceneManager.RecvChangeTurn();
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
            //inGameSceneManager.network_eventCardType = BitConverter.ToInt32(DataRecvBuffer, 4);
            int arrSizeBuffer = BitConverter.ToInt32(NewDataRecvBuffer, 4);
            Debug.Log(" 총 받아야하는 배열의 크기는 " + arrSizeBuffer + "입니다");

            for (int i = 0; i < arrSizeBuffer; i++)
            {
                Debug.Log(" " + i + " 번 까지는 정상적으로 넣었습니다. " + (8 + (4 * i)) + "의 부터 형변환 한 값 ::" + BitConverter.ToInt32(NewDataRecvBuffer, (8 + (4 * i))));
                inGameSceneManager.recvTerrainIndex[i] = BitConverter.ToInt32(NewDataRecvBuffer, (8 + (4 * i)));
            }

            inGameSceneManager.RecvTerrainIndex();
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_EVENTCARD_INDEX)
        {
            //inGameSceneManager.network_eventCardType = BitConverter.ToInt32(DataRecvBuffer, 4);
        }

        // Network Exception
        else if (recvType == (int)PROTOCOL.DISCONNECTED_ENEMY_CLIENT)
        {
            //
            // BGM 제가 꺼드리겠습니다.
            GameObject.Find("GameCores").transform.Find("SoundManager").GetComponent<SoundManager>().BGMMaximumVolume = 0;

            // 씬 이동 ( 추후 상대방 탕출에 의한 승리 결과창 및, 씐 이동으로 변경 필요)
            GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.MainUI_SCENE, true, 2);
            //
            //SendData((int)PROTOCOL.DOUBLECHECK_DISCONNECTED_ENEMY_CLIENT);
        }

        recvType = 0;
    }
}
