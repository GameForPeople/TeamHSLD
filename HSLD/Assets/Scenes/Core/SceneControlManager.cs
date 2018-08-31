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

    // Use this for initialization
    void Start()
    {
        // Release!
        nowScene = SCENE_NAME.TITLE_SCENE;
        // Test!
        //nowScene = SCENE_NAME.LOGIN_SCENE;

        gameCore = GameObject.Find("GameCores").gameObject;
    }

    public void ChangeScene(SCENE_NAME InSceneName)
    {
        StartCoroutine(ChangeSceneCoroutine(InSceneName));
    }

    IEnumerator ChangeSceneCoroutine(SCENE_NAME InSceneName)
    {
        AsyncOperation asyncLoad = SceneManager.LoadSceneAsync((int)InSceneName, LoadSceneMode.Additive);

        while(!asyncLoad.isDone)
        {
            yield return null;
        }

        SceneManager.MoveGameObjectToScene(gameCore, SceneManager.GetSceneByBuildIndex((int)InSceneName));

        SceneManager.UnloadSceneAsync((int)nowScene);

        nowScene = InSceneName;
    }
}
