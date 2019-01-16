using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public partial class InGameSceneManager : MonoBehaviour {
    public bool isDrawEmojiSelectUI;
    GameObject emojiSelectUI;
    GameObject emojiButton;

    public int selectedEmojiIndex; // 해당 인덱스를 0으로 변경하는 것은, NetworkManager의 Send에서 책임져야함.

    GameObject[] myEmojiUI = new GameObject[7];
    GameObject[] enemyEmojiUI = new GameObject[7];

    private void StartForEmoji()
    {
        isDrawEmojiSelectUI = false;

        emojiSelectUI = GameObject.Find("Emoji_Canvas").transform.Find("EmojiSelectUI").gameObject;
        emojiSelectUI.SetActive(false);

        emojiButton = GameObject.Find("Emoji_Canvas").transform.Find("Button").gameObject;
        emojiButton.SetActive(true);

        selectedEmojiIndex = 0;

        myEmojiUI[0] = GameObject.Find("Emoji_Canvas").transform.Find("Image_MyEmoji_Back").gameObject;
        enemyEmojiUI[0] = GameObject.Find("Emoji_Canvas").transform.Find("Image_EnemyEmoji_Back").gameObject;

        myEmojiUI[1] = myEmojiUI[0].transform.Find("Image_MyEmoji_1").gameObject;
        myEmojiUI[2] = myEmojiUI[0].transform.Find("Image_MyEmoji_2").gameObject;
        myEmojiUI[3] = myEmojiUI[0].transform.Find("Image_MyEmoji_3").gameObject;
        myEmojiUI[4] = myEmojiUI[0].transform.Find("Image_MyEmoji_4").gameObject;
        myEmojiUI[5] = myEmojiUI[0].transform.Find("Image_MyEmoji_5").gameObject;
        myEmojiUI[6] = myEmojiUI[0].transform.Find("Image_MyEmoji_6").gameObject;

        enemyEmojiUI[1] = enemyEmojiUI[0].transform.Find("Image_Emoji_1").gameObject;
        enemyEmojiUI[2] = enemyEmojiUI[0].transform.Find("Image_Emoji_2").gameObject;
        enemyEmojiUI[3] = enemyEmojiUI[0].transform.Find("Image_Emoji_3").gameObject;
        enemyEmojiUI[4] = enemyEmojiUI[0].transform.Find("Image_Emoji_4").gameObject;
        enemyEmojiUI[5] = enemyEmojiUI[0].transform.Find("Image_Emoji_5").gameObject;
        enemyEmojiUI[6] = enemyEmojiUI[0].transform.Find("Image_Emoji_6").gameObject;
    }

    //
    public void UI_OnOffEmojiSelectUI()
    {
        if(isDrawEmojiSelectUI)
        {
            isDrawEmojiSelectUI = false;
            emojiSelectUI.SetActive(false);
        }
        else
        {
            isDrawEmojiSelectUI = true;
            emojiSelectUI.SetActive(true);
        }
    }

    /*
     UI_LocalPlayerPickEmoji
      
        로컬 플레이어가 이모지를 선택했을 때, 해당 이모지의 인덱스가 넘어옴.
     */
    public void UI_LocalPlayerPickEmoji(int InEmojiIndex)
    {
        selectedEmojiIndex = InEmojiIndex;

        // 이모지 셀렉션 UI, Button를 꺼줌.
        UI_OnOffEmojiSelectUI();

        emojiButton.SetActive(false);

        UI_TurnOnMyEmoji(InEmojiIndex);
    }

    //My
    private void UI_TurnOnMyEmoji(int InEmojiIndex)
    {
        myEmojiUI[0].SetActive(true);
        myEmojiUI[InEmojiIndex].SetActive(true);

        StartCoroutine(OffMyEmojiCoroutine(InEmojiIndex));
    }

    IEnumerator OffMyEmojiCoroutine(int InEmojiIndex)
    {
        bool a;

        yield return new WaitForSeconds(3.0f);

        myEmojiUI[0].SetActive(false);
        myEmojiUI[InEmojiIndex].SetActive(false);

        yield return new WaitForSeconds(2.0f);
        emojiButton.SetActive(true);
    }


    // Enemy
    public void NetworkManager_TurnOnEnemyEmoji(int InEmojiIndex)
    {
        enemyEmojiUI[0].SetActive(true);
        enemyEmojiUI[InEmojiIndex].SetActive(true);

        StartCoroutine(OffEnemyEmojiCoroutine(InEmojiIndex));
    }

    public IEnumerator OffEnemyEmojiCoroutine(int InEmojiIndex)
    {
        bool a;

        yield return new WaitForSeconds(3.0f);

        enemyEmojiUI[0].SetActive(false);
        enemyEmojiUI[InEmojiIndex].SetActive(false);
    }
}
