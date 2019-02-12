using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DiceObject : MonoBehaviour
{
    private GameObject diceA_Position;
    private GameObject diceA_Rotation;

    private GameObject diceB_Position;
    private GameObject diceB_Rotation;

    private SoundManager soundManager;

    private bool isOnRoll;      // 지금 구르는 중인 가?

    private Vector3 endPosition;    // 끝나고 카메라 밖으로 치움.
    private Quaternion zeroRotation;

    private Vector3[] A_PositionVectors = new Vector3[32];
    private Vector3[] B_PositionVectors = new Vector3[32];

    //private Vector3/*[]*/ A_RotationVectors /* = new Vector3[90]*/;
    //private Vector3/*[]*/ B_RotationVectors /* = new Vector3[90]*/;
    public Vector3 A_RotationVectors = new Vector3( 11.0f, 11.0f, 11.0f);// 5.625f, 2.8125f, 5.625f);
    public Vector3 B_RotationVectors = new Vector3( 11.0f, 11.0f, 11.0f);// 5.625f, 5.625f, 2.8125f);
    /*
     * DiceSystem_Roll
     *
     *  - 주사위 값을 인자로 받아서 주사위를 굴린다...
     *  - 나중에는 DiceSystem하고, NetworkManager, InGameSceneManager에서 호출될 듯 합니다.
     *   
     *  #0. GameObject.Find("Main Camera").transform.Find("DiceManager").GetComponent<DiceObject>().DiceSystem_Roll(1,1);
     *  
     */
    public void DiceSystem_Roll(int InDiceAIndex, int InDiceBIndex)
    {
        StartCoroutine(RollCoroutine(InDiceAIndex, InDiceBIndex));
    }

    //---------------------------------------------------------------

    // Use this for initialization
    void Start()
    {
        if (GameObject.Find("GameCores") != null)
            soundManager = GameObject.Find("GameCores").transform.Find("SoundManager").GetComponent<SoundManager>();
        else
            soundManager = GameObject.FindWithTag("GameManager").transform.Find("SoundManager").GetComponent<SoundManager>();

        diceA_Position = /*GameObject.Find("DiceManager").*/transform.Find("diceA").gameObject;
        diceA_Rotation = /*GameObject.Find("DiceManager").*/transform.Find("diceA").
            transform.Find("dice").gameObject;

        diceB_Position = /*GameObject.Find("DiceManager").*/transform.Find("diceB").gameObject;
        diceB_Rotation = /*GameObject.Find("DiceManager").*/transform.Find("diceB").
            transform.Find("dice").gameObject;

        endPosition = new Vector3(-10000.0f, -10000.0f, -10000.0f);
        zeroRotation = new Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

        SetDicePosition();
        SetDiceRotation();

        EndDice();
    }

    /*
     *  StartDice()
     * 
     *  - 주사위를 굴리기 전에, 시작위치로 이동하고 0,0,0으로 셋해줌.
     */
    private void StartDice()
    {
        isOnRoll = true;

        diceA_Rotation.transform.localRotation = zeroRotation;
        diceB_Rotation.transform.localRotation = zeroRotation;
    }

    /*
    *  EndDice()
    * 
    *  - 주사위를 굴린 후, 화면에서 안보이도록 던져버림.
    */
    private void EndDice()
    {
        isOnRoll = false;

        //A_move.MovePosition(endPosition);
        diceA_Position.transform.localPosition = endPosition;
        diceA_Rotation.transform.localRotation = zeroRotation;

        //B_move.MovePosition(endPosition);
        diceB_Position.transform.localPosition = endPosition;
        diceB_Rotation.transform.localRotation = zeroRotation;
    }

    /*
     * RollCoroutine
     * 
     *  - 외부에서 호출되는 DiceSystem_Roll 에 대하여, 적절한 프로세스를 수행합니다.
     */
    IEnumerator RollCoroutine(int InDiceAIndex, int InDiceBIndex)
    {
        StartDice();

        DiceSetting(InDiceAIndex, InDiceBIndex);

        soundManager.SFXPlay(soundManager.clips[8], 1.0f);  // 나중에 최적화작업 끝나고, 소리 싱크 적용하기.
        int loopCount = 0;

        while (loopCount < 32)
        {
            RollingDiceWithLoopCount(loopCount);
            ++loopCount; //+= 3; // 나중에 최적화 하기. 1로 바꾸고 30까지;;
            yield return new WaitForSeconds(1.0f / 60.0f);  //PC Test
            //yield return new WaitForFixedUpdate();    // Live Version
            //yield return new WaitForEndOfFrame();
        }

        // 프레임 최적화로 인한 오류 수정
        //diceA_Position.transform.localPosition = A_PositionVectors[29];
        //diceB_Position.transform.localPosition = B_PositionVectors[29];

        yield return new WaitForSeconds(1.0f);
        EndDice();
    }

    /*
    * DiceSetting
    * 
    *  - 주사위를 던지기 전에, 해당 숫자로 먼저 주사위를 로컬로 회전해 셋팅해줍니다.
    */
    private void DiceSetting(int InDiceAIndex, int InDiceBIndex)
    {
        //return;

        if (InDiceAIndex == 1)
        //    { }
        diceA_Rotation.transform.Rotate(new Vector3(180, -90, 0), Space.Self);

        else if (InDiceAIndex == 2)
        //   diceA_Rotation.transform.Rotate(new Vector3(90, 90, 0), Space.Self);
        diceA_Rotation.transform.Rotate(new Vector3(-90, 0, 0), Space.Self);

        else if (InDiceAIndex == 3)
        //    diceA_Rotation.transform.Rotate(new Vector3(-90, 0, 0), Space.Self);
        diceA_Rotation.transform.Rotate(new Vector3(180, 0, 0), Space.Self);

        else if (InDiceAIndex == 4)
        //    diceA_Rotation.transform.Rotate(new Vector3(90, 0, 0), Space.Self);
            { }

        else if (InDiceAIndex == 5)
        //    diceA_Rotation.transform.Rotate(new Vector3(180, -90, 0), Space.Self);
        diceA_Rotation.transform.Rotate(new Vector3(90, 0, 0), Space.Self);

        else if (InDiceAIndex == 6)
        //    diceA_Rotation.transform.Rotate(new Vector3(180, 0, 0), Space.Self);
        diceA_Rotation.transform.Rotate(new Vector3(90, 90, 0), Space.Self);


        if (InDiceBIndex == 1)
        //    diceB_Rotation.transform.Rotate(new Vector3(90, 0, 0), Space.Self);
        diceB_Rotation.transform.Rotate(new Vector3(-90, -90, 0), Space.Self);

        else if (InDiceBIndex == 2)
        //    diceB_Rotation.transform.Rotate(new Vector3(180, 0, 0), Space.Self);
        diceB_Rotation.transform.Rotate(new Vector3(-90, 0, 0), Space.Self);

        else if (InDiceBIndex == 3)
        //    diceB_Rotation.transform.Rotate(new Vector3(90, 90, 0), Space.Self);
        diceB_Rotation.transform.Rotate(new Vector3(180, 0, 0), Space.Self);

        else if (InDiceBIndex == 4)
        //    diceB_Rotation.transform.Rotate(new Vector3(-90, -90, 0), Space.Self);
        { }

        else if (InDiceBIndex == 5)
        //    { }
        diceB_Rotation.transform.Rotate(new Vector3(90, 0, 0), Space.Self);

        else if (InDiceBIndex == 6)
        //    diceB_Rotation.transform.Rotate(new Vector3(-90, 0, 0), Space.Self);
        diceB_Rotation.transform.Rotate(new Vector3(90, 90, 0), Space.Self);

    }

    /*
    * RollingDiceWithLoopCount
    * 
    *  - Tick Loop Count에 따라, 다른 위치와 다른 회전값을 주사위에 적용해줍니다.
    */
    private void RollingDiceWithLoopCount(int InLoopCount)
    {
        diceA_Position.transform.localPosition = A_PositionVectors[InLoopCount];
        diceB_Position.transform.localPosition = B_PositionVectors[InLoopCount];

        diceA_Rotation.transform.Rotate(A_RotationVectors/*[InLoopCount]*/, Space.Self);
        diceB_Rotation.transform.Rotate(B_RotationVectors/*[InLoopCount]*/, Space.Self);
    }

    /*
    * SetDicePosition
    * 
    *  - 최초 Dice Pos를 세팅..해줍니다..
    */
    private void SetDicePosition()
    {
        // A주사위 시작 위치 -> 85.0f, 20.0f, -120.0f
        // A주사위 최고 높이 위치 -> 25.0f, 28.0f, -140.0f
        // A주사위 바닥 닿는 위치 -> 5.0f , 10.0f, -160.0f
        // A주사위 끝 위치 -> -5.0f, 0.0f, -170.0f

        // B주사위 시작 위치 -> 85.0f, 20.0f, -120.0f
        // B주사위 최고 높이 위치 -> 25.0f, 28.0f, -140.0f
        // B주사위 바닥 닿는 위치 -> 5.0f , 10.0f, -160.0f
        // B주사위 끝 위치 -> -5.0f, 0.0f, -170.0f


        A_PositionVectors[0] = new Vector3(85.0f, 20.00f, -120.0f);
        A_PositionVectors[1] = new Vector3(79.0f, 20.75f, -122.1f);
        A_PositionVectors[2] = new Vector3(73.0f, 21.50f, -124.2f);
        A_PositionVectors[3] = new Vector3(67.0f, 22.25f, -126.3f);

        A_PositionVectors[4] = new Vector3(61.0f, 23.00f, -128.4f);
        A_PositionVectors[5] = new Vector3(55.0f, 23.75f, -130.5f);
        A_PositionVectors[6] = new Vector3(49.0f, 24.50f, -132.6f);

        A_PositionVectors[7] = new Vector3(43.0f, 25.25f, -134.7f);
        A_PositionVectors[8] = new Vector3(37.0f, 26.00f, -136.8f);
        A_PositionVectors[9] = new Vector3(31.0f, 26.75f, -138.9f);

        // A주사위 최고 높이 위치 -> 25.0f, 28.0f, -140.0f

        A_PositionVectors[10] = new Vector3(26.35f, 28.0f, -141.00f);
        A_PositionVectors[11] = new Vector3(24.40f, 27.7f, -142.60f);
        A_PositionVectors[12] = new Vector3(22.45f, 27.2f, -143.30f);
        A_PositionVectors[13] = new Vector3(20.50f, 26.6f, -144.30f);

        A_PositionVectors[14] = new Vector3(18.55f, 25.7f, -145.50f);
        A_PositionVectors[15] = new Vector3(16.60f, 24.7f, -147.00f);
        A_PositionVectors[16] = new Vector3(14.65f, 23.5f, -148.50f);

        A_PositionVectors[17] = new Vector3(12.70f, 22.1f, -152.20f);
        A_PositionVectors[18] = new Vector3(10.75f, 20.6f, -154.20f);
        A_PositionVectors[19] = new Vector3(08.80f, 18.8f, -157.30f);

        A_PositionVectors[20] = new Vector3(07.00f, 16.9f, -159.40f);
        A_PositionVectors[21] = new Vector3(05.8f, 14.8f, -161.50f);
        A_PositionVectors[22] = new Vector3(04.6f, 12.6f, -163.20f);
        A_PositionVectors[23] = new Vector3(03.4f, 10.5f, -164.70f);

        A_PositionVectors[24] = new Vector3(2.2f, 8.7f, -165.90f);
        A_PositionVectors[25] = new Vector3(1.0f, 7.0f, -167.00f);
        A_PositionVectors[27] = new Vector3(-0.8f, 5.5f, -167.80f);

        A_PositionVectors[28] = new Vector3(-2.2f, 4.0f, -168.36f);
        A_PositionVectors[29] = new Vector3(-3.4f, 2.5f, -168.84f);

        A_PositionVectors[30] = new Vector3(-4.6f, 1.0f, -169.50f);
        A_PositionVectors[31] = new Vector3(-5.4f, 0.0f, -170.00f);
        // A주사위 끝 위치 -> -5.0f, 0.0f, -170.0f


        // B주사위 시작 위치 -> 85.0f, 20.0f, -100.0f
        // B주사위 최고 높이 위치 -> 40.0f, 30.0f, -140.0f
        // B주사위 바닥 닿는 위치 -> 20.0f , 10.0f, -155.0f
        // B주사위 끝 위치 -> 5.0f, 0.0f, -165.0f

        B_PositionVectors[0] = new Vector3(85.0f, 20.00f, -100.0f);
        B_PositionVectors[1] = new Vector3(82.0f, 20.75f, -103.0f);
        B_PositionVectors[2] = new Vector3(79.0f, 21.50f, -106.0f);
        B_PositionVectors[3] = new Vector3(76.0f, 22.25f, -109.0f);
        B_PositionVectors[4] = new Vector3(73.0f, 23.00f, -112.0f);
        B_PositionVectors[5] = new Vector3(70.0f, 23.75f, -115.0f);
        B_PositionVectors[6] = new Vector3(67.0f, 24.50f, -118.0f);
        B_PositionVectors[7] = new Vector3(64.0f, 25.25f, -121.0f);
        B_PositionVectors[8] = new Vector3(61.0f, 26.00f, -124.0f);
        B_PositionVectors[9] = new Vector3(58.0f, 26.75f, -127.0f);

        B_PositionVectors[10] = new Vector3(54.5f, 28.0f, -130.0f);
        B_PositionVectors[11] = new Vector3(50.0f, 28.75f, -133.0f);
        B_PositionVectors[12] = new Vector3(45.5f, 29.5f, -136.0f);
        B_PositionVectors[13] = new Vector3(41.25f, 30.25f, -139.0f);

        // B주사위 최고 높이 위치 -> 40.0f, 30.0f, -140.0f

        B_PositionVectors[14] = new Vector3(38.1f, 28.1f, -141.0f);
        B_PositionVectors[15] = new Vector3(36.2f, 26.2f, -142.5f);
        B_PositionVectors[16] = new Vector3(34.3f, 24.3f, -144.0f);
        B_PositionVectors[17] = new Vector3(32.3f, 22.3f, -145.5f);
        B_PositionVectors[18] = new Vector3(30.4f, 20.4f, -147.0f);
        B_PositionVectors[19] = new Vector3(28.4f, 18.4f, -148.5f);
        B_PositionVectors[20] = new Vector3(26.5f, 16.5f, -150.0f);
        B_PositionVectors[21] = new Vector3(24.5f, 14.5f, -151.5f);
        B_PositionVectors[22] = new Vector3(22.6f, 12.6f, -153.0f);
        B_PositionVectors[23] = new Vector3(20.6f, 10.7f, -154.5f);

        // B주사위 바닥 닿는 위치 -> 20.0f , 10.0f, -155.0f

        B_PositionVectors[24] = new Vector3(17.75f, 9.0f, -156.0f);
        B_PositionVectors[25] = new Vector3(15.5f, 7.5f, -157.5f);
        B_PositionVectors[26] = new Vector3(13.25f, 6.0f, -159.0f);
        B_PositionVectors[27] = new Vector3(11.0f, 4.5f, -160.5f);
        B_PositionVectors[28] = new Vector3(8.75f, 3.0f, -162.0f);
        B_PositionVectors[29] = new Vector3(6.5f, 1.5f, -163.5f);
        B_PositionVectors[30] = new Vector3(5.75f, 1.0f, -164.0f);
        B_PositionVectors[31] = new Vector3(5.0f, 0.5f, -164.5f);
        // B주사위 끝 위치 -> 5.0f, 0.0f, -165.0f
    }

    /*
    * SetDiceRotation
    * 
    *  - 최초 Dice 로테이션을 세팅 해줍니다...
    */
    private void SetDiceRotation()
    {
        //int A_RotateBuffer = 123;
        //int B_RotateBuffer = 100;

        //for (int i = 0; i < 90; ++i)
        //   A_RotationVectors[i] = new Vector3((A_RotateBuffer - i) * 0.09f, (A_RotateBuffer - i) * 0.105f, (A_RotateBuffer - i) * 0.02f);
        //
        //for (int i = 0; i < 90; ++i)
        //   B_RotationVectors[i] = new Vector3((B_RotateBuffer - i) * 0.12f, (B_RotateBuffer - i) * 0.06f, (B_RotateBuffer - i) * 0.13f);

        //A_RotationVectors = new Vector3( 2.995f, 2.5f, 1.55f);
        //B_RotationVectors = new Vector3( 1.0f, 3.5f, 2.1f);
    }
}
