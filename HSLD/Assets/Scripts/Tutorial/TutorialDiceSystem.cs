using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TutorialDiceSystem : MonoBehaviour
{
    public Slider diceSlider;
    public static int getDiceNum;
    public GameObject pointer;
    private int diceValue;
    static public bool isDouble;                //주사위가 더블이 나왔을때
    private int rndTmpValue;
    private bool isTriggerEnter = false;
    private float gaze;
    private bool raiseValue = true;
    public float constValue;
    private int characterDiceRateUp = 5;        //tmp

    static public TutorialDiceSystem instance_;

    private Color initColor = Color.yellow;
    private Color desColor = Color.red;

    static public bool isDiceDouble = false;            //이벤트카드로 인해 주사위를 더블할때
    private float time_;

    private TutorialFlowSystem flowSystem;
    private MissionManager missionManager;

    private void Start()
    {
        instance_ = this;
        flowSystem = GameObject.FindWithTag("GameManager").GetComponent<TutorialFlowSystem>();
        missionManager = GameObject.FindWithTag("GameManager").GetComponent<MissionManager>();
    }

    public void OnTrigger()
    {
        if (!flowSystem.currentFlow.Equals(FLOW.TO_ROLLINGDICE))
            return;

        //init
        diceSlider.value = 0;
        gaze = 0;
        time_ = 0;
        isTriggerEnter = true;
    }
    public void OffTrigger()
    {
        if (!flowSystem.currentFlow.Equals(FLOW.TO_ROLLINGDICE))
            return;
        
        isTriggerEnter = false;
        RollingDice();
    }

    //랜덤 넘버 추출
    int OutRndNum(int min, int max)
    {
        return Random.Range(min, max);
    }

    //슬라이드 값을 바탕으로 다이스 눈 설정.
    public void RollingDice()
    {
        if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().currentTutorial.Equals(TUTORIAL.INGAME_ROLLINGDICE))
        {
            getDiceNum = 66;
            isDouble = false;
        }
        else if (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().currentTutorial.Equals(TUTORIAL.INGAME_DICEFOCUS))
        {
            getDiceNum = 34;
            isDouble = false;
            GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().DoingTutorial(TUTORIAL.INGAME_ROLLINGDICE_2);
        }
        else if(GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().currentTutorial.Equals(TUTORIAL.INGAME_BEFOREATTACK))
        {
            GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().pointObj[2].SetActive(true);
            GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().pointObj[2].transform.localPosition = new Vector3(2, -312, 0);
            GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().pointObj[2].transform.localScale = new Vector3(1.18f, 1.18f, 1.18f);
            getDiceNum = 23;
            isDouble = false;
        }

        else
            return;

        CameraController.DiceCount = ((int)(getDiceNum / 10) + (int)(getDiceNum % 10));
        GameObject.Find("DiceManager").GetComponent<DiceObject>().DiceSystem_Roll(getDiceNum / 10, getDiceNum % 10);

        //flow 변경
        flowSystem.FlowChange(FLOW.TO_ROLLINGDICE);
    }

    //슬라이드 값 설정
    void SetSliderValue()
    {
        if (diceSlider.value == diceSlider.maxValue)
            raiseValue = false;

        if (diceSlider.value == diceSlider.minValue)
            raiseValue = true;

        if (raiseValue)
            gaze += (Time.deltaTime * constValue);
        else
            gaze -= (Time.deltaTime * constValue);

        diceSlider.value = gaze;
        pointer.transform.localEulerAngles = Vector3.Lerp(new Vector3(0, 0, 92), new Vector3(0, 0, -95), gaze);
        //gameObject.GetComponent<Image>().color = Color.Lerp(initColor, desColor, gaze);
        //diceSlider.fillRect.GetComponent<Image>().color = Color.Lerp(initColor, desColor, gaze);
    }

    private void Update()
    {
        if (isTriggerEnter && flowSystem.currentFlow.Equals(FLOW.TO_ROLLINGDICE) && (GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().currentTutorial.Equals(TUTORIAL.INGAME_ROLLINGDICE) || GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().currentTutorial.Equals(TUTORIAL.INGAME_DICEFOCUS) || GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().currentTutorial.Equals(TUTORIAL.INGAME_BEFOREATTACK)))
        {
            time_ += Time.deltaTime;
            if (time_ > 6f)
            {
                Debug.Log("예외처리 : 주사위 클릭했는데 6초가 지났을때");
                isTriggerEnter = false;
                RollingDice();
            }
            if (Input.GetMouseButton(0))
            {
                SetSliderValue();
            }
        }
        else if (!isTriggerEnter)
            time_ = 0;
    }

    private void OnDisable()
    {
        gaze = 0;
        diceSlider.value = gaze;
    }
}
