using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.UI;

public partial class SceneControlManager : MonoBehaviour {

    GameObject loadUI;
    GameObject sliderUI;
    GameObject printingTextUI;

    GameObject babyChickenUI;
    GameObject skiingPenguinUI;
    GameObject updownSnakeUI;
    GameObject dancingWhaleUI;

    IEnumerator CoroutineInstance_loadUI;
    IEnumerator CoroutineInstance_SliderUI;


    /*
     DrawOnlyLoadUI
        UI기획안에 따라, 씐 전환없이 LoadUI의 출력이 요구되므로, 관련 기능의 함수를 제공합니다.

         #0. 인자는 다음과 같습니다.
            - LOAD_UI_TYPE InLoadUIIndex : 출력하기 원하는 LOAD_UI_TYPE enum값. (Default : LOAD_UI_TYPE.FALSE)
            - float InUIPrintTime : 출력하기 원하는 시간.  (Default : 2.0f)

        #1. 사용 예는 다음과 같습니다.

            - GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().DrawOnlyLoadUI();

            a. 임의의 로딩UI를 기본 시간(2초간) 시간만큼 출력하고자할 때 : DrawOnlyLoadUI(); 
            b. 희망하는 타입의 로딩UI를 기본 시간(2초간) 출력하고자할 때 : DrawOnlyLoadUI( LOAD_UI_TYPE.SKIING_PENGUIN );
            d. 임의의 로딩UI를 5초 간 출력하고자할 때 : DrawOnlyLoadUI( LOAD_UI_TYPE.FALSE, 5.0f );
            e. 희망하는 타입의 로딩UI를 5초 간 출력하고자할 때 : DrawOnlyLoadUI( LOAD_UI_TYPE.SKIING_PENGUIN, 5.0f );
    */
    public IEnumerator DrawOnlyLoadUI(LOAD_UI_TYPE InLoadUIIndex = LOAD_UI_TYPE.FALSE, float InUIPrintTime = 2.0f)
    {
        InLoadUIIndex = ActiveLoadingUI(InLoadUIIndex, InUIPrintTime);

        yield return new WaitForSeconds(InUIPrintTime);

        InactiveLoadingUI(InLoadUIIndex);
    }

    // Use this for initialization
    public void StartForLoadUI() {
        loadUI = GameObject.Find("GameCores").transform.Find("LoadUI").gameObject;

        printingTextUI = loadUI.transform.Find("Text_Edit").gameObject;
        sliderUI = loadUI.transform.Find("Slider").gameObject;

        babyChickenUI = loadUI.transform.Find("BabyChickenUI").gameObject;
        skiingPenguinUI = loadUI.transform.Find("SkiingPenguinUI").gameObject;
        updownSnakeUI = loadUI.transform.Find("UpdownSnakeUI").gameObject;
        dancingWhaleUI = loadUI.transform.Find("DancingWhaleUI").gameObject;
    }

    /*
     * ActiveLoadingUI
     *  - 인자로 주어진 인덱스의 로딩 UI를 인자로 주어진 시간만큼 활성화 시킵니다.
     *  
     *  !0. InLoadUIIndex값으로  LOAD_UI_TYPE.FALSE가 올 때, 랜덤을 보장합니다.
     */
    public LOAD_UI_TYPE ActiveLoadingUI(LOAD_UI_TYPE InLoadUIIndex, float InUIPrintTime)
    {
        loadUI.SetActive(true);

        // Default 값일 경우, 랜덤으로 UI를 선택합니다.
        if(InLoadUIIndex == LOAD_UI_TYPE.FALSE)
        {
            InLoadUIIndex = (LOAD_UI_TYPE)((int)Random.Range(0, 1));
        }

        // 씐 전환 이전에, 먼저 LoadingUI를 출력합니다.
        if (InLoadUIIndex == LOAD_UI_TYPE.BABY_CHICKEN)
            CoroutineInstance_loadUI = ActiveBabyChicken();
        else if (InLoadUIIndex == LOAD_UI_TYPE.SKIING_PENGUIN)
            CoroutineInstance_loadUI = ActiveSkiingPenguin();
        else if (InLoadUIIndex == LOAD_UI_TYPE.UPDOWN_SNAKE)
            CoroutineInstance_loadUI = ActiveUpdownSnake();
        else if (InLoadUIIndex == LOAD_UI_TYPE.DANCING_WHALE)
            CoroutineInstance_loadUI = ActiveDancingWhale();

        StartCoroutine(CoroutineInstance_loadUI);

        CoroutineInstance_SliderUI = SliderUICoroutine(InUIPrintTime);
        StartCoroutine(CoroutineInstance_SliderUI);

        return InLoadUIIndex;
    }

    public void InactiveLoadingUI(LOAD_UI_TYPE InLoadUIIndex)
    {
        loadUI.SetActive(false);
        StopCoroutine(CoroutineInstance_loadUI);
        StopCoroutine(CoroutineInstance_SliderUI);

        if (InLoadUIIndex == LOAD_UI_TYPE.BABY_CHICKEN)
            babyChickenUI.SetActive(false);
        else if (InLoadUIIndex == LOAD_UI_TYPE.SKIING_PENGUIN)
            skiingPenguinUI.SetActive(false);
        else if (InLoadUIIndex == LOAD_UI_TYPE.UPDOWN_SNAKE)
            updownSnakeUI.SetActive(false);
        else if (InLoadUIIndex == LOAD_UI_TYPE.DANCING_WHALE)
            dancingWhaleUI.SetActive(false);
    }

    private IEnumerator SliderUICoroutine(float InUIPrintTime)
    {
        float sliderValue = 0;

        // 30프로까지 가는데, 절반의 시간이 걸리고
        while (sliderValue < 0.3f)
        {
            sliderValue += 0.01f;
            sliderUI.GetComponent<Slider>().value = sliderValue;

            yield return new WaitForSeconds((InUIPrintTime / 2) / 30);
        }

        float plusRatio = 1.0f;

        // 나머지 시간동안 70프로를 딱딱 끊어서 가게 구현. 완벽히 맞을 필요 없음.
        while (true)
        {
            yield return new WaitForSeconds((InUIPrintTime / 2) / 7);

            sliderValue += 0.15f * plusRatio;
            plusRatio += Random.Range(0.2f, 0.8f);

            if (sliderValue > 1.0f) sliderValue = 1.0f;

            sliderUI.GetComponent<Slider>().value = sliderValue;
        }
    }

    private IEnumerator ActiveBabyChicken()
    {
        babyChickenUI.SetActive(true);

        GameObject image_babyChicken = babyChickenUI.transform.Find("Image_BabyChicken").gameObject;
        Vector2 babyChickenPos = new Vector2(0.0f, 30.0f);
        image_babyChicken.transform.localPosition = babyChickenPos;

        while (true)
        {
            while (babyChickenPos.y <= 70.0f)
            {
                babyChickenPos.y += 8.0f;
                image_babyChicken.transform.localPosition = babyChickenPos;
                yield return new WaitForSeconds(1.0f / 30.0f);
            }

            while (babyChickenPos.y >= 30.0f)
            {
                babyChickenPos.y -= 8.0f;
                image_babyChicken.transform.localPosition = babyChickenPos;
                yield return new WaitForSeconds(1.0f / 30.0f);
            }

            yield return new WaitForSeconds( 0.3f );
        }
    }

    private IEnumerator ActiveUpdownSnake()
    {
        updownSnakeUI.SetActive(true);

        GameObject image_snake = updownSnakeUI.transform.Find("Image_UpdownSnake").gameObject;
        Vector2 snakePos = new Vector2(0.0f, 25.0f);
        image_snake.transform.localPosition = snakePos;

        while (true)
        {
            while (snakePos.y <= 45.0f)
            {
                snakePos.y += 4.0f;
                image_snake.transform.localPosition = snakePos;
                yield return new WaitForSeconds(1.0f / 30.0f);
            }

            yield return new WaitForSeconds(0.3f);

            while (snakePos.y >= 25.0f)
            {
                snakePos.y -= 4.0f;
                image_snake.transform.localPosition = snakePos;
                yield return new WaitForSeconds(1.0f / 30.0f);
            }

            yield return new WaitForSeconds(0.3f);
        }
    }

    private IEnumerator ActiveSkiingPenguin()
    {
        yield return new WaitForSeconds(0.3f);
    }

    private IEnumerator ActiveDancingWhale()
    {
        yield return new WaitForSeconds(0.3f);
    }
}
