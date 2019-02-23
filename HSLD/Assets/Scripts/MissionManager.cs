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
    static public int selectedMainMissionIndex;
    static public int selectedSubMissionIndex;
    static public bool[] missionCompleteBoolean = new bool[5];
    static public bool[] enemyMissionCompleteBoolean = new bool[5];
    static public int missionComplete = 0;
    static public int enemyMissionComplete = 0;

    private NetworkManager networkManager;

    //중복되지않게 메인미션 / 서브미션 부여
    private void RndMainMissionSet(int index)
    {
        //init
        missionSet[index].mainMission.ResetCurrentCnt();
        readyDisplayMainMissionObj.transform.GetChild(0).GetComponent<Text>().text = missionSet[index].mainMission.text;
        readyDisplayMainMissionObj.transform.GetChild(0).GetComponent<Text>().text += "( " + missionSet[index].mainMission.currentCnt + " / " + missionSet[index].mainMission.goalCnt + " )";
    }

    private void RndSubMissionSet(int index)
    {
        for (int i = 0; i < missionSet[index].subMission.Length; i++)
            missionSet[index].subMission[i].ResetCurrentCnt();

        for (int i = 0; i < 5; i++)
        {
            readyDisplaySubMissionObj.transform.GetChild(i).GetComponent<Text>().text = missionSet[index].subMission[i].text;
            readyDisplaySubMissionObj.transform.GetChild(i).GetComponent<Text>().text += "( " + missionSet[index].subMission[i].currentCnt + " / " + missionSet[index].subMission[i].goalCnt + " )";
        }
    }

    private void Start()
    {
        // 서버에서 가져오기
        if (GameObject.Find("GameCores") != null)
        {
            selectedMainMissionIndex = GameObject.Find("NetworkManager").GetComponent<NetworkManager>().playerMissionIndex;
            selectedSubMissionIndex = GameObject.Find("NetworkManager").GetComponent<NetworkManager>().subMissionIndex;
        }
        else
        {
            selectedMainMissionIndex = UnityEngine.Random.Range(0, missionSet.Length);
            selectedSubMissionIndex = UnityEngine.Random.Range(0, missionSet.Length);
        }

        for (int i = 0; i < missionCompleteBoolean.Length; i++)
            missionCompleteBoolean[i] = false;
        for (int i = 0; i < enemyMissionCompleteBoolean.Length; i++)
            enemyMissionCompleteBoolean[i] = false;
        for(int i =0; i < 5 ;i++)
            readyDisplaySubMissionObj.transform.GetChild(i).transform.GetChild(0).gameObject.SetActive(false);

        missionCanvas = gameObject.GetComponent<FlowSystem>().missionSetParentTransform.gameObject;
        RndMainMissionSet(selectedMainMissionIndex);
        RndSubMissionSet(selectedSubMissionIndex);
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
        readyDisplayMainMissionObj.transform.GetChild(0).GetComponent<Text>().text = missionSet[selectedMainMissionIndex].mainMission.text;
        readyDisplayMainMissionObj.transform.GetChild(0).GetComponent<Text>().text += "( " + missionSet[selectedMainMissionIndex].mainMission.currentCnt + " / " + missionSet[selectedMainMissionIndex].mainMission.goalCnt + " )";

        for (int i =1; i<6;i++)
        {
            readyDisplaySubMissionObj.transform.GetChild(i - 1).GetComponent<Text>().text = missionSet[selectedSubMissionIndex].subMission[i-1].text;
            readyDisplaySubMissionObj.transform.GetChild(i - 1).GetComponent<Text>().text += "( " + missionSet[selectedSubMissionIndex].subMission[i - 1].currentCnt + " / " + missionSet[selectedMainMissionIndex].subMission[i - 1].goalCnt + " )";
        }
    }

    //한 턴 카운팅할때 사용
    public void MainMissionCounting(int val)
    {
        if (missionSet[selectedMainMissionIndex].mainMission.currentCnt + val >= missionSet[selectedMainMissionIndex].mainMission.goalCnt)
            missionSet[selectedMainMissionIndex].mainMission.currentCnt = missionSet[selectedMainMissionIndex].mainMission.goalCnt;
        else
            missionSet[selectedMainMissionIndex].mainMission.currentCnt += val;

        ResetMissionDisplay();
    }

    //한 턴 카운팅할때 사용
    public void SubMissionCounting(int val, int index)
    {
        if (enemyMissionCompleteBoolean[index])
            return;

            if (missionSet[selectedSubMissionIndex].subMission[index].currentCnt + val >= missionSet[selectedSubMissionIndex].subMission[index].goalCnt)
        {
            if (!missionCompleteBoolean[index])
            {
                missionComplete += 1;
                missionCompleteBoolean[index] = true;
            }

            missionSet[selectedSubMissionIndex].subMission[index].currentCnt = missionSet[selectedSubMissionIndex].subMission[index].goalCnt;
        }

        else
            missionSet[selectedSubMissionIndex].subMission[index].currentCnt += val;

        ResetMissionDisplay();
        //if (identify.Equals(Identify.ALLY))
        //{
            
        //}
        //else if (identify.Equals(Identify.ENEMY))
        //{
        //    if (missionEnemyCurrentCnt[index] + val >= missionSet[selectedSubMissionIndex].subMission[index].goalCnt)
        //    {
        //        if (!missionCompleteEnemyBoolean[index])
        //        {
        //            enemyMissionComplete += 1;
        //            missionCompleteEnemyBoolean[index] = true;
        //        }

        //        missionEnemyCurrentCnt[index] = missionSet[selectedSubMissionIndex].subMission[index].goalCnt;
        //    }

        //    else
        //        missionEnemyCurrentCnt[index] += val;
        //}
    }

    //누적카운팅할때 사용
    public void MainMissionContinuedCounting(int val)
    {
        if (missionSet[selectedMainMissionIndex].mainMission.currentCnt + val >= missionSet[selectedMainMissionIndex].mainMission.goalCnt)
            missionSet[selectedMainMissionIndex].mainMission.currentCnt = missionSet[selectedMainMissionIndex].mainMission.goalCnt;
        else
            missionSet[selectedMainMissionIndex].mainMission.currentCnt = val;

        ResetMissionDisplay();
    }

    //누적카운팅할때 사용
    public void SubMissionContinuedCounting(int val, int index)
    {
        if (enemyMissionCompleteBoolean[index])
            return;

        if (missionSet[selectedSubMissionIndex].subMission[index].currentCnt + val >= missionSet[selectedSubMissionIndex].subMission[index].goalCnt)
        {
            if (!missionCompleteBoolean[index])
            {
                missionComplete += 1;
                missionCompleteBoolean[index] = true;
            }

            missionSet[selectedSubMissionIndex].subMission[index].currentCnt = missionSet[selectedSubMissionIndex].subMission[index].goalCnt;
        }

        else
            missionSet[selectedSubMissionIndex].subMission[index].currentCnt = val;

        ResetMissionDisplay();

        //if (identify.Equals(Identify.ALLY))
        //{
            
        //}
        //else if (identify.Equals(Identify.ENEMY))
        //{
        //    if (missionEnemyCurrentCnt[index] + val >= missionSet[selectedSubMissionIndex].subMission[index].goalCnt)
        //    {
        //        if (!missionCompleteEnemyBoolean[index])
        //        {
        //            enemyMissionComplete += 1;
        //            missionCompleteEnemyBoolean[index] = true;
        //        }

        //        missionEnemyCurrentCnt[index] = missionSet[selectedSubMissionIndex].subMission[index].goalCnt;
        //    }

        //    else
        //        missionEnemyCurrentCnt[index] = val;
        //}
    }
    public int EnemyMainMissionCounting()
    {
        int cnt = 0;

        //메인미션 101번, 비옥지형 100칸이상 설치
        if (GameObject.Find("NetworkManager").GetComponent<NetworkManager>().enemyMissionIndex == 0)
            cnt = gameObject.GetComponent<TerrainGainCounting>().ModerationCounting(Identify.ENEMY);

        //메인미션 102번, 한랭지형 100칸이상 설치
        if (GameObject.Find("NetworkManager").GetComponent<NetworkManager>().enemyMissionIndex == 1)
            cnt = gameObject.GetComponent<TerrainGainCounting>().ColdCounting(Identify.ENEMY);

        //메인미션 103번, 척박지형 100칸이상 설치
        if (GameObject.Find("NetworkManager").GetComponent<NetworkManager>().enemyMissionIndex == 2)
            cnt = gameObject.GetComponent<TerrainGainCounting>().BarrenCounting(Identify.ENEMY);

        return cnt;
    }
}
