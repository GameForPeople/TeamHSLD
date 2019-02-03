using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

[Serializable]
public class Mission
{
    public MissionInfo mainMission;
    public MissionInfo[] subMission = new MissionInfo[5];
}

public class MissionManager : MonoBehaviour
{
    //public MissionInfo[] missionSet;
    public Mission[] missionSet;

    public GameObject readyDisplayMainMissionObj;
    public GameObject readyDisplaySubMissionObj;

    public float missionCanvasActiveTime;
    private Vector3 openCanvasVec = new Vector3(-78, 0,0);
    private Vector3 closeCanvasVec = new Vector3(-576, 0, 0);

    private bool isActive = false;
    private bool isTrigger = false;
    private float time_ = 0;

    private GameObject missionCanvas;
    static public int selectedIndex;

    //중복되지않게 메인미션 / 서브미션 부여
    private void RndMissionSet(int index)
    {
        //init
        missionSet[index].mainMission.ResetCurrentCnt();
        for (int i = 0; i < missionSet[index].subMission.Length; i++)
            missionSet[index].subMission[i].ResetCurrentCnt();

        readyDisplayMainMissionObj.transform.GetChild(0).GetComponent<Text>().text = missionSet[index].mainMission.text;
        readyDisplayMainMissionObj.transform.GetChild(0).GetComponent<Text>().text += "( " + missionSet[index].mainMission.currentCnt + " / " + missionSet[index].mainMission.goalCnt + " )";

        for (int i = 0; i < 5; i++)
        {
            readyDisplaySubMissionObj.transform.GetChild(i).GetComponent<Text>().text = missionSet[index].subMission[i].text;
            readyDisplaySubMissionObj.transform.GetChild(i).GetComponent<Text>().text += "( " + missionSet[index].subMission[i].currentCnt + " / " + missionSet[index].subMission[i].goalCnt + " )";
        }
    }

    private void Start()
    {
        missionCanvas = gameObject.GetComponent<FlowSystem>().missionSetParentTransform.gameObject;
        selectedIndex = UnityEngine.Random.Range(0, missionSet.Length);
        selectedIndex = 0;
        RndMissionSet(selectedIndex);
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
        readyDisplayMainMissionObj.transform.GetChild(0).GetComponent<Text>().text = missionSet[selectedIndex].mainMission.text;
        readyDisplayMainMissionObj.transform.GetChild(0).GetComponent<Text>().text += "( " + missionSet[selectedIndex].mainMission.currentCnt + " / " + missionSet[selectedIndex].mainMission.goalCnt + " )";

        for (int i =1; i<6;i++)
        {
            readyDisplaySubMissionObj.transform.GetChild(i - 1).GetComponent<Text>().text = missionSet[selectedIndex].subMission[i-1].text;
            readyDisplaySubMissionObj.transform.GetChild(i - 1).GetComponent<Text>().text += "( " + missionSet[selectedIndex].subMission[i - 1].currentCnt + " / " + missionSet[selectedIndex].subMission[i - 1].goalCnt + " )";
        }
    }
}
