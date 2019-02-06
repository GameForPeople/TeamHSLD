using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.UI;

public partial class MainUISceneManager : MonoBehaviour
{
    public int nowCharacterIndex;

    GameObject DetailUserDataUI;
    GameObject UserDataUI;

    string[] characterName = new string[9];
    //bool[] enableCharacterArr = new bool [9];
    int[] characterBit = new int[9];

    public int selectedCharacterIndex;  // 플레이어가 선택한 캐릭터의 인덱스

    private void StartForCharacter()
    {
        nowCharacterIndex = networkObject.activeCharacterIndex;
        selectedCharacterIndex = 0;

        DetailUserDataUI = GameObject.Find("UserDataUI").transform.Find("Canvas_Dynamic").transform.Find("DetailUserDataUI").gameObject;
        UserDataUI = GameObject.Find("UserDataUI").gameObject;

        characterName[0] = "My Planet";
        characterName[1] = "승리의 아이콘";
        characterName[2] = "해의 캐릭터";
        characterName[3] = "땅신";
        characterName[4] = "풀신";
        characterName[5] = "물신";
        characterName[6] = "불신";
        characterName[7] = "얼음신";
        characterName[8] = "나무신";

        characterBit[0] = 0;
        characterBit[1] = (1 << 0);
        characterBit[2] = (1 << 1);
        characterBit[3] = (1 << 2);
        characterBit[4] = (1 << 3);
        characterBit[5] = (1 << 4);
        characterBit[6] = (1 << 5);
        characterBit[7] = (1 << 6);
        characterBit[8] = (1 << 7);

        RefreshUserDataUI();
        SetUseorNotUseCharacterUI();

        // 저장되면 해당 인덱스 네트워크 매니저에서 받아서 체크해줘야함
        ChangeActiveCharacter(nowCharacterIndex, false);
    }

    /*
     RefreshUserDataUI
     
     UserDataUI를 갱신합니다. 돈, 닉네임을 다시 네트워크에서 받아와서 해결합니다.
     */
    private void RefreshUserDataUI()
    {
        // 닉네임 설정
        UserDataUI.transform.Find("Canvas").transform.Find("NickName_Text").gameObject.GetComponent<Text>().text
             = networkObject.nickName.ToString();

        // 돈 설정
        UserDataUI.transform.Find("Canvas").transform.Find("Money_Text").gameObject.GetComponent<Text>().text
             = networkObject.money.ToString();
    }

    /*
        ChangeActiveCharacter를 UI에서 호출하기 위해 1번 래핑한 함수입니다. 항상 서버에 변경사항을 전달해야합니다.
    */
    public void UI_ChangeActiveCharacter(int InNewCharacterIndex)
    {
        soundManager.SFXPlay(soundManager.clips[1], 1.0f);

        ChangeActiveCharacter(InNewCharacterIndex, true);
    }

    /*
        ChangeActiveCharacter

        단순히 현재 장착중인 캐릭터를 로컬에서 변경합니다. (추후 네트워크 통신 필요할수도? )
        Image가 눌릴 때 실행됩니다! (NotUse일경우에는 구매 함수 활용)
    */
    public void ChangeActiveCharacter(int InNewCharacterIndex, bool InSendPacketToServer /* = false*/)
    {
        string strBuf = "Image_";

        Debug.Log("[DEBUG] 현재 선택된 캐릭터 인덱스 : " + nowCharacterIndex + " 뿌시면? : " + strBuf + nowCharacterIndex.ToString());

        // 디테일 내부, 활성화 캐릭터 UI 변경.
        GameObject character_Set = DetailUserDataUI.transform.Find("Character_Set").gameObject;
        character_Set.transform.Find(strBuf + nowCharacterIndex.ToString()).transform.Find("Image_Use").gameObject.SetActive(false);
        character_Set.transform.Find(strBuf + InNewCharacterIndex.ToString()).transform.Find("Image_Use").gameObject.SetActive(true);

        // 유저 정보 UI에서의 변경.
        GameObject Image_Character_Set = UserDataUI.transform.Find("Canvas").transform.Find("Image_Character_Set").gameObject;
        Image_Character_Set.transform.Find(strBuf + nowCharacterIndex.ToString()).gameObject.SetActive(false);
        Image_Character_Set.transform.Find(strBuf + InNewCharacterIndex.ToString()).gameObject.SetActive(true);

        // 캐릭터 인덱스 변경.
        nowCharacterIndex = InNewCharacterIndex;
        networkObject.activeCharacterIndex = nowCharacterIndex;

        if(InSendPacketToServer)
            networkObject.SendData(PROTOCOL.CHANGE_ACTIVE_CHARACTER);
    }

    /*
     UI_OnBuyCharacterUI

        잠금된 캐릭터를 클릭했을 떄 관련 상점 UI가 출력됩니다.
    */
    public void UI_OnBuyCharacterUI(int InNewCharacterIndex)
    {
        soundManager.SFXPlay(soundManager.clips[2], 1.0f);

        selectedCharacterIndex = InNewCharacterIndex;

        GameObject buyCharacterUI = UserDataUI.transform.Find("Canvas_Dynamic").transform.Find("BuyCharacterUI").gameObject;

        buyCharacterUI.SetActive(true);

        buyCharacterUI.transform.Find("Text_CharacterName").GetComponent<Text>().text = characterName[InNewCharacterIndex];
        buyCharacterUI.transform.Find("Text_YourMoney").GetComponent<Text>().text = networkObject.money.ToString();
        buyCharacterUI.transform.Find("Image_Character_Set").transform.Find("Image_" + InNewCharacterIndex.ToString()).gameObject.SetActive(true);
    }

    public void UI_OffBuyCharacterUI()
    {
        soundManager.SFXPlay(soundManager.clips[3], 1.0f);

        GameObject buyCharacterUI = UserDataUI.transform.Find("Canvas_Dynamic").transform.Find("BuyCharacterUI").gameObject;

        buyCharacterUI.transform.Find("Image_Character_Set").transform.Find("Image_" + selectedCharacterIndex.ToString()).gameObject.SetActive(false);
        buyCharacterUI.SetActive(false);
    }

    /*
        UI_BuyCharacter

        Buy Character 버튼을 눌럿을 때 실행되는 함수입니다, 인덱스는 selectedCharacterIndex 입니다.
    */
    public void UI_BuyCharacter()
    {
        Debug.Log("Function : UI_BuyCharacter");
        networkObject.SendData(PROTOCOL.DEMAND_BUY_CHARACTER);
    }

    /*
        NetworkManager_AnswerBuyCharacter

        캐릭터를 구매 요청했을 때, 

        // 인자값으로 들어온 int값이, -1이면 성공, 0이면 돈없어서 실패, 1이면 이미 있는 캐릭터이여서 실패, 2면 이런 캐릭터 안팔아요!(네트워크 에러 가능성)
    */
    public void NetworkManager_AnswerBuyCharacter(int InIndex)
    {
        Debug.Log("Function : NetworkManager_AnswerBuyCharacter");

        UI_OffBuyCharacterUI();

        Debug.Log("서버로 부터 받은 값은? [ -1:성공, 0:실패(돈), 1:실패(이미소유), 2:실패(네트워크에러) ] : " + InIndex.ToString());

        // -1이면 성공, 0이면 돈없어서 실패, 1이면 이미 있는 캐릭터이여서 실패, 2면 이런 캐릭터 안팔아요!(네트워크 에러 가능성)
        if (InIndex == -1)
        {
            Debug.Log("구매 전 캐릭터 비트는 : " + networkObject.characterBit.ToString());

            networkObject.characterBit ^= characterBit[selectedCharacterIndex];
            networkObject.money -= 1000;

            Debug.Log("구매 후 캐릭터 비트는 : " + networkObject.characterBit.ToString());

            StartCoroutine(AnswerBuyCharacterUICoroutine(true));

            // 구매한 캐릭터를 활성화 캐릭터로 변경.
            ChangeActiveCharacter(selectedCharacterIndex, true);
            RefreshUserDataUI();
            SetUseorNotUseCharacterUI();
        }
        else if (InIndex == 0)
        {
            StartCoroutine(AnswerBuyCharacterUICoroutine(false));
        }
        else if (InIndex == 1)
        {
            StartCoroutine(AnswerBuyCharacterUICoroutine(false));
        }
    }

    /*
    AnswerBuyCharacterUICoroutine

    캐릭터 구매 성공, 혹은 실패를 받았을 때, 이를 UI를 통해 화면에 출력할 때 사용하는 함수.
    */
    public IEnumerator AnswerBuyCharacterUICoroutine(bool isSuccess)
    {
        Debug.Log("Function : AnswerBuyCharacterUICoroutine");

        GameObject successOrFailUI;

        if (isSuccess)
        {
            PlayParticle(1);
            soundManager.SFXPlay(soundManager.clips[16], 1.0f);

            successOrFailUI = UserDataUI.transform.Find("Canvas_Popup").transform.Find("SuccessBuyCharacterUI").gameObject;
            successOrFailUI.SetActive(true);

            //GameObject.Find("FX_Canvas").GetComponent<MainUISceneFXManager>().RenderSuccessOrFailParticle(true);
        }
        else
        {
            PlayParticle(2);
            soundManager.SFXPlay(soundManager.clips[16], 1.0f);

            successOrFailUI = UserDataUI.transform.Find("Canvas_Popup").transform.Find("FailBuyCharacterUI").gameObject;
            successOrFailUI.SetActive(true);
            //GameObject.Find("FX_Canvas").GetComponent<MainUISceneFXManager>().RenderSuccessOrFailParticle(false);
        }

        yield return new WaitForSeconds(1.0f);

        if (isSuccess)
        {
            successOrFailUI.transform.Find("OnOff").gameObject.SetActive(true);
            successOrFailUI.transform.Find("OnOff").transform.Find("Image_Character_Set").transform.Find("Image_" + selectedCharacterIndex.ToString() ).gameObject.SetActive(true);

            //GameObject.Find("FX_Canvas").GetComponent<MainUISceneFXManager>().RenderSuccessOrFailParticle(true);
        }
        else
        {
            successOrFailUI.transform.Find("OnOff").gameObject.SetActive(true);
            //GameObject.Find("FX_Canvas").GetComponent<MainUISceneFXManager>().RenderSuccessOrFailParticle(false);
        }

        yield return new WaitForSeconds(2.0f);

        if (isSuccess)
        {
            successOrFailUI.transform.Find("OnOff").Find("Image_Character_Set").transform.Find("Image_" + selectedCharacterIndex.ToString()).gameObject.SetActive(false);
            successOrFailUI.transform.Find("OnOff").gameObject.SetActive(false);
            successOrFailUI.SetActive(false);
        }
        else
        {
            successOrFailUI.transform.Find("OnOff").gameObject.SetActive(false);
            successOrFailUI.SetActive(false);
        }
    }

    /*
     UI_DetailUserDataUI

        DetailUserDataUI를 OnOff하는 함수입니다.
    */
    public void UI_DetailUserDataUI(bool InBValue)
    {
        if (InBValue == true)
        {
            soundManager.SFXPlay(soundManager.clips[2], 1.0f);

            DetailUserDataUI.transform.Find("WinText").GetComponent<Text>().text = networkObject.winCount.ToString();
            DetailUserDataUI.transform.Find("LoseText").GetComponent<Text>().text = networkObject.loseCount.ToString();

            DetailUserDataUI.SetActive(true);
        }
        else if (InBValue == false)
        {
            soundManager.SFXPlay(soundManager.clips[3], 1.0f);

            DetailUserDataUI.SetActive(false);
        }
    }

    /*
     SetUseorNotUseCharacter
        
        UI_DetailUserDataUI가 호출될 때, UseUI, NotUseUI의 Active 여부를 조절한다.

        여기서 이를 판별하기 위해, NetworkManager의 CharacterBit를 사용하는데, 이 뜻은,
        캐릭터 구매가 있을 경우, 동기화를 위해 CharacterBit를 변경하고 이 함수를 호출을해야함을 뜻함.
     */
    private void SetUseorNotUseCharacterUI()
    {
        GameObject imageCharacterSet = DetailUserDataUI.transform.Find("Character_Set").gameObject;
        string strBuf = "Image_";
        string cstrBuf = "Image_NotUse";

        for (int i = 0; i < 9; ++i)
        {
            if ((networkObject.characterBit & characterBit[i]) == characterBit[i])
            {
                imageCharacterSet.transform.Find(strBuf + i.ToString()).transform.Find(cstrBuf).gameObject.SetActive(false);
            }
            else
            {
                imageCharacterSet.transform.Find(strBuf + i.ToString()).transform.Find(cstrBuf).gameObject.SetActive(true);
            }
        }
    }
}
