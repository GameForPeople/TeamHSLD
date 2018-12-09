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

    public void SendData(int InMsg)
    {
        StartCoroutine(SendDataCoroutine(InMsg));
    }

    public IEnumerator SendDataCoroutine(int InMsg)
    {
        while (networkSyncLock)
        {
            yield return new WaitForSeconds(0.1f); //현재 다른 함수에 의해 네트워크 통신중인지를 확인합니다.
        }
        networkSyncLock = true;    // 다른 네트워크 함수의 통신을 억제합니다.

        if (NetworkInterface.GetIsNetworkAvailable())
        {
            if (isOnNetwork)
            {
                //LoginScene
                if (InMsg == (int)PROTOCOL.DEMAND_LOGIN)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_LOGIN), 0, NewDataSendBuffer, 0, 4);

                    ID = GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().IDBuffer;
                    int idSize = ID.Length;

                    Debug.Log(" " + ID + "는 입력한 ID 값, ID Size =>> " + idSize);

                    Buffer.BlockCopy(BitConverter.GetBytes(idSize), 0, NewDataSendBuffer, 4, 4);
                    //Buffer.BlockCopy(BitConverter.GetBytes(ID), 0, DataSendBuffer, 8, idSize);
                    //for (int iter = 0; iter < idSize; iter++)
                    //{
                    //    DataSendBuffer[8 + iter] = (byte)ID[iter];
                    //}
                    Buffer.BlockCopy(Encoding.Default.GetBytes(ID), 0, NewDataSendBuffer, 8, idSize);

                    socket.Send(NewDataSendBuffer, 8 + idSize, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.CHANGE_NICKNAME)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.CHANGE_NICKNAME), 0, NewDataSendBuffer, 0, 4);

                    int nickNameSize = nickName.Length * 2; //DEV_66

                    Debug.Log(" " + nickName + "는 입력한 nickName 값, nickName Size =>> " + nickNameSize);

                    Buffer.BlockCopy(BitConverter.GetBytes(nickNameSize), 0, NewDataSendBuffer, 4, 4);

                    Buffer.BlockCopy(Encoding.Unicode.GetBytes(nickName), 0, NewDataSendBuffer, 8, nickNameSize);

                    socket.Send(NewDataSendBuffer, 8 + nickNameSize, SocketFlags.None);
                }

                // Main UI
                else if (InMsg == (int)PROTOCOL.DEMAND_FRIEND_INFO)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_FRIEND_INFO), 0, NewDataSendBuffer, 0, 4);
                    socket.Send(NewDataSendBuffer, 4, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.DEMAND_FRIEND_INVITE)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_FRIEND_INVITE), 0, NewDataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(
                        GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().invitedFriendIndex
                        ), 0, NewDataSendBuffer, 4, 4);

                    socket.Send(NewDataSendBuffer, 8, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.ANSWER_FRIEND_INVITE)
                {
                    isHost = false;

                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.ANSWER_FRIEND_INVITE), 0, NewDataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(
                        GameObject.Find("GameCores").transform.Find("CoreUIManager").GetComponent<CoreUIManager>().answerFriendInviteValue
                        ), 0, NewDataSendBuffer, 4, 4);

                    socket.Send(NewDataSendBuffer, 8, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.DELAY_FRIEND_INVITE)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DELAY_FRIEND_INVITE), 0, NewDataSendBuffer, 0, 4);

                    socket.Send(NewDataSendBuffer, 4, SocketFlags.None);
                }

                else if (InMsg == (int)PROTOCOL.DEMAND_MAKE_FRIEND)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_MAKE_FRIEND), 0, NewDataSendBuffer, 0, 4);
                    string stringBuffer = GameObject.Find("MainUISceneManager").GetComponent<MainUISceneManager>().makeFriendIDBuffer;
                    int stringSize = stringBuffer.Length;

                    Buffer.BlockCopy(BitConverter.GetBytes(stringSize), 0, NewDataSendBuffer, 4, 4);
                    Buffer.BlockCopy(Encoding.Default.GetBytes(stringBuffer), 0, NewDataSendBuffer, 8, stringSize);

                    socket.Send(NewDataSendBuffer, 8 + stringSize, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.DEMAND_MAKE_FRIEND_INFO)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_MAKE_FRIEND_INFO), 0, NewDataSendBuffer, 0, 4);
                    socket.Send(NewDataSendBuffer, 4, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.ANSWER_MAKE_FRIEND)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.ANSWER_MAKE_FRIEND), 0, NewDataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(
                        GameObject.Find("GameCores").transform.Find("CoreUIManager").GetComponent<CoreUIManager>().answerFriendMakeValue
                        ), 0, NewDataSendBuffer, 4, 4);

                    socket.Send(NewDataSendBuffer, 8, SocketFlags.None);
                }
                //LobbyScene - old
                //else if (InMsg == (int)PROTOCOL.DEMAND_MAKEROOM)
                //else if (InMsg == (int)PROTOCOL.DEMAND_JOINROOM)

                //LobbyScene - new
                else if (InMsg == (int)PROTOCOL.DEMAND_RANDOM_MATCH)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_RANDOM_MATCH), 0, NewDataSendBuffer, 0, 4);
                    socket.Send(NewDataSendBuffer, 4, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.DEMAND_GUEST_JOIN)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_GUEST_JOIN), 0, NewDataSendBuffer, 0, 4);
                    socket.Send(NewDataSendBuffer, 4, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.DEMAND_EXIT_RANDOM)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_EXIT_RANDOM), 0, NewDataSendBuffer, 0, 4);
                    socket.Send(NewDataSendBuffer, 4, SocketFlags.None);
                }
                // Lobby Friend Function
                else if (InMsg == (int)PROTOCOL.DEMAND_FRIEND_JOIN)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_FRIEND_JOIN), 0, NewDataSendBuffer, 0, 4);
                    socket.Send(NewDataSendBuffer, 4, SocketFlags.None);
                }
                // RoomScene - old
                //else if (InMsg == (int)PROTOCOL.DEMAND_ROOMHOST)

                // RoomScene - new
                else if (InMsg == (int)PROTOCOL.DEMAND_ENEMY_CHARACTER)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_ENEMY_CHARACTER), 0, NewDataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(playerCharacterIndex), 0, NewDataSendBuffer, 4, 4);

                    socket.Send(NewDataSendBuffer, 8, SocketFlags.None);
                }

                //InGameScene // Defense Turn
                else if (InMsg == (int)PROTOCOL.VOID_GAME_STATE)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.VOID_GAME_STATE), 0, NewDataSendBuffer, 0, 4);
                    socket.Send(NewDataSendBuffer, 4, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.NOTIFY_CHANGE_TURN)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.NOTIFY_CHANGE_TURN), 0, NewDataSendBuffer, 0, 4);
                    socket.Send(NewDataSendBuffer, 4, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.NOTIFY_DICE_VALUE)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.NOTIFY_DICE_VALUE), 0, NewDataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_changeTerrainCount), 0, NewDataSendBuffer, 4, 4);

                    socket.Send(NewDataSendBuffer, 8, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.NOTIFY_TERRAIN_TYPE)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.NOTIFY_TERRAIN_TYPE), 0, NewDataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_terrainType), 0, NewDataSendBuffer, 4, 4);

                    //Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_terrainType), 0, DataSendBuffer, 4, 4);
                    //Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_changeTerrainCount), 0, DataSendBuffer, 8, 4);
                    //Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_terrainIndex[0]), 0, DataSendBuffer, 12, 4 * inGameSceneManager.network_changeTerrainCount);

                    socket.Send(NewDataSendBuffer, 8, SocketFlags.None);  // 70..? 나중에 계산하기..!
                }
                else if (InMsg == (int)PROTOCOL.NOTIFY_TERRAIN_INDEXS)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.NOTIFY_TERRAIN_INDEXS), 0, NewDataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_changeTerrainCount), 0, NewDataSendBuffer, 4, 4);

                    Debug.Log(" " + inGameSceneManager.network_changeTerrainCount + " 이 주사위 값, 적재해야하는 인덱스의 크기입니다.");

                    int iBuffer;
                    for (int i = 0; i < inGameSceneManager.network_changeTerrainCount; i++)
                    {
                        iBuffer = inGameSceneManager.network_terrainIndex[i];
                        Buffer.BlockCopy(BitConverter.GetBytes(iBuffer), 0, NewDataSendBuffer, (8 + 4 * i), 4);
                        Debug.Log(" " + i + " 번 까지는 정상적으로 적재했습니다. 값 " + BitConverter.ToInt32(NewDataSendBuffer, (8 + 4 * i)));
                    }

                    //Debug.Log(" 전송하는 메모리의 크기는 ==> " + (8 + 4 * inGameSceneManager.network_changeTerrainCount) + " == " + BitConverter.ToInt32(NewDataSendBuffer, 4));

                    socket.Send(NewDataSendBuffer, 80, SocketFlags.None);  // 70..? 나중에 계산하기;;

                }
                else if (InMsg == (int)PROTOCOL.NOTIFY_EVENTCARD_INDEX)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.NOTIFY_EVENTCARD_INDEX), 0, NewDataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_eventCardType), 0, NewDataSendBuffer, 4, 4);

                    socket.Send(NewDataSendBuffer, 8, SocketFlags.None);
                }
                // Network Exception
                else if (InMsg == (int)PROTOCOL.DOUBLECHECK_DISCONNECTED_ENEMY_CLIENT)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.NOTIFY_CHANGE_TURN), 0, NewDataSendBuffer, 0, 4);
                    socket.Send(NewDataSendBuffer, 4, SocketFlags.None);
                }

                else if (InMsg == (int)PROTOCOL.NOTIFY_GAME_READY)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.NOTIFY_GAME_READY), 0, NewDataSendBuffer, 0, 4);
                    socket.Send(NewDataSendBuffer, 4, SocketFlags.None);
                }

                RecvProcess();
            }
            else
            {
                Debug.Log("주의 : OnlyClientTest 환경에서, SendData가 호출되었습니다.");
            }
        }
        else
        {
            GameObject.Find("GameCores").transform.Find("AppQuitManager").GetComponent<AppQuitManager>().Quit(true, 1);
        }
    }
}
