﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MissionManager : MonoBehaviour
{
    public MissionInfo[] missionSet;
    private bool[] isCheck;
    private int index = 0;

    public GameObject readyDisplayMainMissionObj;
    public GameObject readyDisplaySubMissionObj;

    public float missionCanvasActiveTime;
    private Vector3 openCanvasVec = new Vector3(-37,0,0);
    private Vector3 closeCanvasVec = new Vector3(-576, 0, 0);

    private bool isActive = false;
    private bool isTrigger = false;
    private float time_ = 0;

    private GameObject missionCanvas;
    private int[] indexSaved = new int[6];

    //중복되지않게 메인미션 / 서브미션 부여
    private void randomValue(int min, int max, MissionSet set)
    {
        int value = Random.Range(min, max);
        if (!isCheck[value])
        {
            isCheck[value] = true;
            switch(set)
            {
                case MissionSet.MAIN:
                    readyDisplayMainMissionObj.transform.GetChild(0).GetComponent<Text>().text = missionSet[value].text;
                    readyDisplayMainMissionObj.transform.GetChild(0).GetComponent<Text>().text += "( " + missionSet[value].currentCnt + " / " + missionSet[value].goalCnt + " )";
                    indexSaved[0] = value;
                    break;
                case MissionSet.SUB:
                    
                    readyDisplaySubMissionObj.transform.GetChild(index).GetComponent<Text>().text = missionSet[value].text;
                    readyDisplaySubMissionObj.transform.GetChild(index).GetComponent<Text>().text += "( " + missionSet[value].currentCnt + " / " + missionSet[value].goalCnt + " )";
                    //inGameDisplaySubMissionObj.transform.GetChild(1 + index).transform.GetChild(0).GetComponent<Text>().text = missionSet[value].text;
                    index += 1;
                    indexSaved[index] = value;
                    break;
            }
        }
            
        else
            randomValue(min, max, set);            
    }

    private void Start()
    {
        missionCanvas = gameObject.GetComponent<FlowSystem>().missionSetParentTransform.gameObject;
        isCheck = new bool[missionSet.Length];
        for (int i = 0; i < isCheck.Length; i++)
            isCheck[i] = false;

        randomValue(0, 5, MissionSet.MAIN);

        for (int i = 0; i < 5; i++)
            randomValue(5, 20, MissionSet.SUB);
    }

    public void OnClick()
    {
        if (isTrigger)
            return;

        if (isActive)
            StartCoroutine(InActive());
        else
            StartCoroutine(Active());
    }

    private IEnumerator Active()
    {
        ResetMissionDisplay();
        isActive = true;
        time_ = 0;
        isTrigger = true;

        while(true)
        {
            time_ += Time.deltaTime;
            yield return new WaitForEndOfFrame();
            missionCanvas.transform.localPosition = Vector3.Lerp(missionCanvas.transform.localPosition, openCanvasVec, time_);
            if (time_ > missionCanvasActiveTime)
                break;
        }
        missionCanvas.transform.localPosition = openCanvasVec;
        isTrigger = false;
        
    }

    private IEnumerator InActive()
    {
        isActive = false;
        time_ = 0;
        isTrigger = true;

        while (true)
        {
            time_ += Time.deltaTime;
            yield return new WaitForEndOfFrame();
            missionCanvas.transform.localPosition = Vector3.Lerp(missionCanvas.transform.localPosition, closeCanvasVec,time_);
            if (time_ > missionCanvasActiveTime)
                break;
        }
        missionCanvas.transform.localPosition = closeCanvasVec;
        isTrigger = false;
    }

    public void ResetMissionDisplay()
    {
        readyDisplayMainMissionObj.transform.GetChild(0).GetComponent<Text>().text = missionSet[indexSaved[0]].text;
        readyDisplayMainMissionObj.transform.GetChild(0).GetComponent<Text>().text += "( " + missionSet[indexSaved[0]].currentCnt + " / " + missionSet[indexSaved[0]].goalCnt + " )";

        for (int i =1; i<indexSaved.Length;i++)
        {
            readyDisplaySubMissionObj.transform.GetChild(i - 1).GetComponent<Text>().text = missionSet[indexSaved[i]].text;
            readyDisplaySubMissionObj.transform.GetChild(i - 1).GetComponent<Text>().text += "( " + missionSet[indexSaved[i]].currentCnt + " / " + missionSet[indexSaved[i]].goalCnt + " )";
        }
    }
}
