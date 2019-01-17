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
    bool[] enableCharacterArr = new bool [9];

    private int selectedCharacterIndex;  // 플레이어가 선택한 캐릭터의 인덱스

    private void StartForCharacter()
    {
        nowCharacterIndex = 0;
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

        // 닉네임 설정
        UserDataUI.transform.Find("Canvas").transform.Find("NickName_Text").gameObject.GetComponent<Text>().text
             = networkObject.nickName.ToString();

        // 돈 설정
        UserDataUI.transform.Find("Canvas").transform.Find("Money_Text").gameObject.GetComponent<Text>().text
             = networkObject.money.ToString();
    }

    /*
        UI_ChangeActiveCharacter

        단순히 현재 장착중인 캐릭터를 로컬에서 변경합니다. (추후 네트워크 통신 필요할수도? )
        Image가 눌릴 때 실행됩니다! (NotUse일경우에는 구매 함수 활용)
    */
    public void UI_ChangeActiveCharacter(int InNewCharacterIndex)
    {
        string strBuf = "Image_";

        // 디테일 내부 캐릭터 변경.
        GameObject character_Set = DetailUserDataUI.transform.Find("Character_Set").gameObject;
        character_Set.transform.Find(strBuf + nowCharacterIndex.ToString()).transform.Find("Image_Use").gameObject.SetActive(false);
        character_Set.transform.Find(strBuf + InNewCharacterIndex.ToString()).transform.Find("Image_Use").gameObject.SetActive(true);

        // 유저 정보 UI에서의 변경.
        GameObject Image_Character_Set = UserDataUI.transform.Find("Canvas").transform.Find("Image_Character_Set").gameObject;
        Image_Character_Set.transform.Find(strBuf + nowCharacterIndex.ToString()).gameObject.SetActive(false);
        Image_Character_Set.transform.Find(strBuf + InNewCharacterIndex.ToString()).gameObject.SetActive(true);

        // 캐릭터 인덱스 변경.
        nowCharacterIndex = InNewCharacterIndex;
    }

    /*
     UI_OnBuyCharacterUI

        잠금된 캐릭터를 클릭했을 떄 관련 상점 UI가 출력됩니다.
    */
    public void UI_OnBuyCharacterUI(int InNewCharacterIndex)
    {
        selectedCharacterIndex = InNewCharacterIndex;

        GameObject buyCharacterUI = UserDataUI.transform.Find("Canvas_Dynamic").transform.Find("BuyCharacterUI").gameObject;

        buyCharacterUI.SetActive(true);

        buyCharacterUI.transform.Find("Text_CharacterName").GetComponent<Text>().text = characterName[InNewCharacterIndex];
        buyCharacterUI.transform.Find("Text_YourMoney").GetComponent<Text>().text = networkObject.money.ToString();
        buyCharacterUI.transform.Find("Image_Character_Set").transform.Find("Image_" + InNewCharacterIndex.ToString()).gameObject.SetActive(true);
    }

    public void UI_OffBuyCharacterUI()
    {
        GameObject buyCharacterUI = UserDataUI.transform.Find("Canvas_Dynamic").transform.Find("BuyCharacterUI").gameObject;

        buyCharacterUI.transform.Find("Image_Character_Set").transform.Find("Image_" + selectedCharacterIndex.ToString()).gameObject.SetActive(false);
        buyCharacterUI.SetActive(false);
    }

    public void UI_BuyCharacter()
    {
       

    }


    /*
     UI_DetailUserDataUI

        DetailUserDataUI를 OnOff하는 함수입니다.
    */
    public void UI_DetailUserDataUI(bool InBValue)
    {
        if (InBValue == true)
        {
            DetailUserDataUI.transform.Find("WinText").GetComponent<Text>().text = networkObject.winCount.ToString();
            DetailUserDataUI.transform.Find("LoseText").GetComponent<Text>().text = networkObject.loseCount.ToString();

            DetailUserDataUI.SetActive(true);
        }
        else if (InBValue == false)
        {
            DetailUserDataUI.SetActive(false);
        }
    }

    /*
     SetUseorNotUseCharacter
        
        UI_DetailUserDataUI가 호출될 때, UseUI, NotUseUI의 Active 여부를 조절한다.

        여기서 이를 판별하기 위해, NetworkManager의 CharacterBit를 사용하는데, 이 뜻은,
        캐릭터 구매가 있을 경우, 동기화를 위해 CharacterBit를 변경하고 이 함수를 호출을 뜻함.
     */
    private void SetUseorNotUseCharacter()
    {

    }
}
