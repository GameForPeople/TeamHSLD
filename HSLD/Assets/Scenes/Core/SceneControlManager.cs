using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;


// Enum 의 순서는, 유니티 씐 빌드와 일치시켜줘야합니다.
// 해당 작업을 하지 않을 경우, 문제가 발생할 수 있습니다.

public enum SCENE_NAME //: string
{
    TITLE_SCENE = 0,    // 로고 노출 및 연결
    LOGIN_SCENE = 1,    // 계정 로그인
    MainUI_SCENE = 2,       // 게임 메인 UI 씐
    LOBBY_SCENE = 3,    // 이거 쫌 그래 사실 별로야
    ROOM_SCENE = 4,     // 방으로 연결
    INGAME_SCENE = 5	// 얍얍얍 인게임 얍얍얍
};

public class SceneControlManager : MonoBehaviour
{

    public SCENE_NAME nowScene;
    GameObject gameCore;
    GameObject[] loadUI = new GameObject[5];

    // Use this for initialization
    void Start()
    {
        // Release!
        nowScene = SCENE_NAME.TITLE_SCENE;
        // Test!
        //nowScene = SCENE_NAME.LOGIN_SCENE;

        gameCore = GameObject.Find("GameCores").gameObject;

        loadUI[0] = GameObject.Find("GameCores").transform.Find("Canvas").transform.Find("LoadUI_0").gameObject;
        loadUI[1] = GameObject.Find("GameCores").transform.Find("Canvas").transform.Find("LoadUI_1").gameObject;
        loadUI[2] = GameObject.Find("GameCores").transform.Find("Canvas").transform.Find("LoadUI_2").gameObject;
        loadUI[3] = GameObject.Find("GameCores").transform.Find("Canvas").transform.Find("LoadUI_3").gameObject;
        loadUI[4] = GameObject.Find("GameCores").transform.Find("Canvas").transform.Find("LoadUI_4").gameObject;

        for(int i = 0; i < 5; ++i)
        {
            loadUI[i].SetActive(false);
        }
    }

    //public void ChangeScene(SCENE_NAME InSceneName)
    //{
    //    StartCoroutine(ChangeSceneCoroutine(InSceneName));
    //}

    public void ChangeScene(SCENE_NAME InSceneName, bool InOnPrintLoadUI = false, int InLoadUIIndex = -1)
    {
        StartCoroutine(ChangeSceneCoroutine(InSceneName, InOnPrintLoadUI, InLoadUIIndex));
    }

    public IEnumerator ChangeSceneCoroutine(SCENE_NAME InSceneName, bool InOnPrintLoadUI, int InLoadUIIndex)
    {
        //Load UI의 출력여부를 먼저 확인합니다.
        if(InOnPrintLoadUI == true)
        {
            // 지정된 이미지가 없을 경우, 랜덤으로 선택하여 출력합니다.
            if(InLoadUIIndex == -1)
            {
                InLoadUIIndex = Random.Range(0, 5);
            }

            // 씐 전환 이전에, 먼저 이미지를 출력해줍니다.
            loadUI[InLoadUIIndex].SetActive(true);
        }

        AsyncOperation asyncLoad = SceneManager.LoadSceneAsync((int)InSceneName, LoadSceneMode.Additive);

        // 로딩이 완료될 때 까지, 대기상태입니다.
        while (!asyncLoad.isDone)
        {
            yield return null;
        }

        SceneManager.MoveGameObjectToScene(gameCore, SceneManager.GetSceneByBuildIndex((int)InSceneName));

        SceneManager.UnloadSceneAsync((int)nowScene);
        nowScene = InSceneName;


        // 씬 변환이 끝나면, LoadUI를 Off처리 해줍니다.
        if (InOnPrintLoadUI == true)
        {
             // Load UI를 어느정도 확인할 시간을 마련해 줍니다.
            yield return new WaitForSeconds(2.0f);

            loadUI[InLoadUIIndex].SetActive(false);
        }

    }
}
