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

public enum LOAD_UI_TYPE : int
{
    FALSE = -1,
    BABY_CHICKEN = 0,
    SKIING_PENGUIN = 1,
    UPDOWN_SNAKE = 2,
    DANCING_WHALE = 3
}

public partial class SceneControlManager : MonoBehaviour
{
    public bool isOnNetwork;
    public SCENE_NAME nowScene;
    GameObject gameCore;

    // Use this for initialization
    void Start()
    {
        isOnNetwork = false;

        // Release!
        nowScene = SCENE_NAME.TITLE_SCENE;
        // Test!
        //nowScene = SCENE_NAME.LOGIN_SCENE;

        gameCore = GameObject.Find("GameCores").gameObject;
        StartForLoadUI();
    }

    //public void ChangeScene(SCENE_NAME InSceneName)
    //{
    //    StartCoroutine(ChangeSceneCoroutine(InSceneName));
    //}

    public void StartNetworkFunction()
    {
        gameCore.transform.Find("NetworkManager").GetComponent<NetworkManager>().StartNetworkFunction();
    }

    /*
        ChangeScene
        - 씬을 변경하는 함수입니다.

        #0. 인자는 다음과 같습니다.
            - SCENE_NAME InNextSceneName : 해당 함수를 호출해서 전환하기 원하는 다음 씬 enum 값
            - bool InOnPrintLoadUI : Loading UI 출력 여부 (Default : false)
            - LOAD_UI_TYPE InLoadUIIndex : 출력하기 원하는 LOAD_UI_TYPE enum값.
            - float InUIPrintTime : 출력하기 원하는 시간.

        #1. 사용 예는 다음과 같습니다.
            a. 로딩UI를 출력하지 않고, 씐 변환만 희망할 때 : ChangeScene(SCENE_NAME.MainUI_SCENE); 
            b. 임의의 로딩UI를 출력하고, 씐 변환할 때 : ChangeScene(SCENE_NAME.MainUI_SCENE, true); 
            c. 희망하는 타입의 로딩UI를 기본 시간(2초간) 출력하고, 씐 변환할 때 : ChangeScene(SCENE_NAME.MainUI_SCENE, true, LOAD_UI_TYPE.BABY_CHICKEN); 
            d. 희망하는 타입의 로딩UI를 5초간 출력하고, 씐 변환할 때 : ChangeScene(SCENE_NAME.MainUI_SCENE, true, LOAD_UI_TYPE.SKIING_PENGUIN, 5.0f); 
            e. 임의의 로딩UI를 3초간 출력하고, 씐 변환할 때 : ChangeScene(SCENE_NAME.MainUI_SCENE, true, LOAD_UI_TYPE.FALSE, 3.0f); 
    */
    public void ChangeScene(SCENE_NAME InNextSceneName, bool InOnPrintLoadUI = false, LOAD_UI_TYPE InLoadUIIndex = LOAD_UI_TYPE.FALSE, float InUIPrintTime = 2.0f)
    {
        StartCoroutine(ChangeSceneCoroutine(InNextSceneName, InOnPrintLoadUI, InLoadUIIndex, InUIPrintTime));
    }

    public IEnumerator ChangeSceneCoroutine(SCENE_NAME InNextSceneName, bool InOnPrintLoadUI, LOAD_UI_TYPE InLoadUIIndex, float InUIPrintTime)
    {
        //Load UI의 출력여부를 먼저 확인합니다.
        if (InOnPrintLoadUI == true)
        {
            InLoadUIIndex = ActiveLoadingUI(InLoadUIIndex, InUIPrintTime);
        }

        AsyncOperation asyncLoad = SceneManager.LoadSceneAsync((int)InNextSceneName, LoadSceneMode.Additive);

        // 로딩이 완료될 때 까지, 대기상태입니다.
        while (!asyncLoad.isDone)
        {
            yield return null;
        }

        //SceneManager.MoveGameObjectToScene(gameCore, SceneManager.GetSceneByBuildIndex((int)InSceneName));

        SceneManager.UnloadSceneAsync((int)nowScene);
        nowScene = InNextSceneName;

        // 씬 변환이 끝나면, LoadUI를 Off처리 해줍니다.
        if (InOnPrintLoadUI == true)
        {
            // Load UI를 어느정도 확인할 시간을 마련해 줍니다.
            yield return new WaitForSeconds(InUIPrintTime);

            InactiveLoadingUI(InLoadUIIndex);
        }

    }
}
