using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AppQuitManager : MonoBehaviour {
    int quitTimeCount;

    GameObject[] quitUI = new GameObject[1];

    void Start()
    {
        GameObject.Find("GameCores").transform.Find("ExitUI_Canvas").gameObject.SetActive(true);

        quitUI[0] = GameObject.Find("GameCores").transform.Find("ExitUI_Canvas").transform.Find("ExitUI_1").gameObject;

        quitUI[0].SetActive(false);
    }

    public void Quit(bool isDrawExitUI = false, int ExitType = 0, int ExitTime = 5)
    {
        if(isDrawExitUI)
        {
            quitTimeCount = ExitTime;

            if (ExitType == 1)
                quitUI[0].SetActive(true);

        }
        else quitTimeCount = ExitTime;

        StartCoroutine("GameExitCoroutine");
    }

    public IEnumerator GameExitCoroutine()
    {
        while (true)
        {
            quitTimeCount--;

            if (quitTimeCount <= 0)
            {
#if UNITY_EDITOR
                UnityEditor.EditorApplication.isPlaying = false; //play모드를 false로.
                                                                 //#elif UNITY_WEBPLAYER
                                                                 //        Application.OpenURL("http://google.com"); //구글웹으로 전환
#else
                        Application.Quit(); //어플리케이션 종료
#endif
            }
            yield return new WaitForSeconds(1.0f);
        }
    }
}
