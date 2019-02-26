using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TerrainGainCounting : MonoBehaviour
{
    int terrainGain = 0;
    int enemyTerrainGain = 0;

    //최적화 스크립트
    private Transform planetTrans;
    private MissionManager missionmanager;

    private void Start()
    {
        planetTrans = GameObject.FindWithTag("InGamePlanet").transform;
        missionmanager = gameObject.GetComponent<MissionManager>();
    }

    //점령한 지형 총 몇개인지 출력
    public int TerrainCounting(Identify identify)
    {
        terrainGain = 0;

        for (int i = 0; i < planetTrans.childCount; i++)
            if (planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(identify))
                terrainGain += 1;

        return terrainGain;
    }

    //비옥지형 총 몇개인지 출력
    public int ModerationCounting(Identify identify)
    {
        terrainGain = 0;

        for (int i = 0; i < planetTrans.childCount; i++)
            if (planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(identify))
                if(planetTrans.GetChild(i).GetComponent<MeshController>().terrainstate.Equals(Terrain.MODERATION))
                    terrainGain += 1;

        return terrainGain;
    }

    //한랭지형 총 몇개인지 출력
    public int ColdCounting(Identify identify)
    {
        terrainGain = 0;

        for (int i = 0; i < planetTrans.childCount; i++)
            if (planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(identify))
                if (planetTrans.GetChild(i).GetComponent<MeshController>().terrainstate.Equals(Terrain.COLD))
                    terrainGain += 1;

        return terrainGain;
    }

    //척박지형 총 몇개인지 출력
    public int BarrenCounting(Identify identify)
    {
        terrainGain = 0;

        for (int i = 0; i < planetTrans.childCount; i++)
            if (planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(identify))
                if (planetTrans.GetChild(i).GetComponent<MeshController>().terrainstate.Equals(Terrain.BARREN))
                    terrainGain += 1;

        return terrainGain;
    }

    //바다지형 총 몇개인지 출력
    public int SeaCounting(Identify identify)
    {
        terrainGain = 0;

        for (int i = 0; i < planetTrans.childCount; i++)
            if (planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(identify))
                if (planetTrans.GetChild(i).GetComponent<MeshController>().terrainstate.Equals(Terrain.SEA))
                    terrainGain += 1;

        return terrainGain;
    }

    //산지형 총 몇개인지 출력
    public int MountainCounting(Identify identify)
    {
        terrainGain = 0;

        for (int i = 0; i < planetTrans.childCount; i++)
            if (planetTrans.GetChild(i).GetComponent<MeshController>().currentIdentify.Equals(identify))
                if (planetTrans.GetChild(i).GetComponent<MeshController>().terrainstate.Equals(Terrain.MOUNTAIN))
                    terrainGain += 1;

        return terrainGain;
    }

    public void CheckFlag()
    {
        //합계가 200개가넘을경우 게임종료
        if (TerrainCounting(Identify.ALLY) >= 200)
            gameObject.GetComponent<InGameSceneManager>().SendGameEnd();

        //메인미션 101번, 비옥지형 100칸이상 설치
        if (MissionManager.selectedMainMissionIndex == 0)
            missionmanager.MainMissionContinuedCounting(ModerationCounting(Identify.ALLY));

        //메인미션 102번, 한랭지형 100칸이상 설치
        if (MissionManager.selectedMainMissionIndex == 1)
            missionmanager.MainMissionContinuedCounting(ColdCounting(Identify.ALLY));

        //메인미션 103번, 척박지형 100칸이상 설치
        if (MissionManager.selectedMainMissionIndex == 2)
            missionmanager.MainMissionContinuedCounting(BarrenCounting(Identify.ALLY));

        //서브미션 201번, 방어지형 10칸이상 설치
        if (MissionManager.selectedSubMissionIndex == 0)
            missionmanager.SubMissionContinuedCounting(MountainCounting(Identify.ALLY) + SeaCounting(Identify.ALLY), 1);

        //서브미션 211번, 한랭지형 30칸이상 설치
        if (MissionManager.selectedSubMissionIndex == 0)
            missionmanager.SubMissionContinuedCounting(ColdCounting(Identify.ALLY), 3);

        //서브미션 411번, 비옥지형 30칸이상 설치
        if (MissionManager.selectedSubMissionIndex == 2)
            missionmanager.SubMissionContinuedCounting(ModerationCounting(Identify.ALLY), 3);

        //서브미션 420번, 한랭지형 30칸이상 설치
        if (MissionManager.selectedSubMissionIndex == 2)
            missionmanager.SubMissionContinuedCounting(ColdCounting(Identify.ALLY), 4);

        //서브미션 501번, 바다지형 15칸이상 설치
        if (MissionManager.selectedSubMissionIndex == 3)
            missionmanager.SubMissionContinuedCounting(SeaCounting(Identify.ALLY), 1);

        //서브미션 511번, 척박지형 20칸이상 설치
        if (MissionManager.selectedSubMissionIndex == 3)
            missionmanager.SubMissionContinuedCounting(BarrenCounting(Identify.ALLY), 3);

        //서브미션 520번, 산지형 30칸이상 설치
        if (MissionManager.selectedSubMissionIndex == 3)
            missionmanager.SubMissionContinuedCounting(MountainCounting(Identify.ALLY), 4);

        //네트워크상태일때
        if (GameObject.Find("GameCores") != null)
            return;

        ////서브미션 201번, 방어지형 10칸이상 설치
        //if (MissionManager.selectedSubMissionIndex == 0)
        //    missionmanager.SubMissionContinuedCounting(MountainCounting(Identify.ENEMY) + SeaCounting(Identify.ENEMY), 1, Identify.ENEMY);

        ////서브미션 211번, 한랭지형 30칸이상 설치
        //if (MissionManager.selectedSubMissionIndex == 0)
        //    missionmanager.SubMissionContinuedCounting(ColdCounting(Identify.ENEMY), 3, Identify.ENEMY);

        ////서브미션 411번, 비옥지형 30칸이상 설치
        //if (MissionManager.selectedSubMissionIndex == 2)
        //    missionmanager.SubMissionContinuedCounting(ModerationCounting(Identify.ENEMY), 3, Identify.ENEMY);

        ////서브미션 420번, 한랭지형 30칸이상 설치
        //if (MissionManager.selectedSubMissionIndex == 2)
        //    missionmanager.SubMissionContinuedCounting(ColdCounting(Identify.ENEMY), 4, Identify.ENEMY);

        ////서브미션 501번, 바다지형 15칸이상 설치
        //if (MissionManager.selectedSubMissionIndex == 3)
        //    missionmanager.SubMissionContinuedCounting(SeaCounting(Identify.ENEMY), 1, Identify.ENEMY);

        ////서브미션 511번, 척박지형 20칸이상 설치
        //if (MissionManager.selectedSubMissionIndex == 3)
        //    missionmanager.SubMissionContinuedCounting(BarrenCounting(Identify.ENEMY), 3, Identify.ENEMY);

        ////서브미션 520번, 산지형 30칸이상 설치
        //if (MissionManager.selectedSubMissionIndex == 3)
        //    missionmanager.SubMissionContinuedCounting(MountainCounting(Identify.ENEMY), 4, Identify.ENEMY);
    }
}
