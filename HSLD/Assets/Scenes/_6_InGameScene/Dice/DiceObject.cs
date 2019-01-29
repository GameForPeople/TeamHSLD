using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DiceObject : MonoBehaviour
{
    private GameObject diceA_Position;
    private GameObject diceA_Rotation;

    private GameObject diceB_Position;
    private GameObject diceB_Rotation;

    private bool isOnRoll;      // 지금 구르는 중인 가?

    private Vector3 endPosition;    // 끝나고 카메라 밖으로 치움.
    private Quaternion zeroRotation;

    private Vector3[] A_PositionVectors = new Vector3[90];
    private Vector3[] B_PositionVectors = new Vector3[90];

    private Vector3[] A_RotationVectors = new Vector3[90];
    private Vector3[] B_RotationVectors = new Vector3[90];

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
        diceA_Position = GameObject.Find("DiceManager").transform.Find("diceA").gameObject;
        diceA_Rotation = GameObject.Find("DiceManager").transform.Find("diceA").
            transform.Find("dice").gameObject;

        diceB_Position = GameObject.Find("DiceManager").transform.Find("diceB").gameObject;
        diceB_Rotation = GameObject.Find("DiceManager").transform.Find("diceB").
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

        int loopCount = 0;
        while (loopCount < 90)
        {
            RollingDiceWithLoopCount(loopCount);
            ++loopCount;
            yield return new WaitForSeconds(1.0f / 200.0f);
        }
        yield return new WaitForSeconds(1.5f);
        EndDice();
    }

    /*
    * DiceSetting
    * 
    *  - 주사위를 던지기 전에, 해당 숫자로 먼저 주사위를 로컬로 회전해 셋팅해줍니다.
    */
    private void DiceSetting(int InDiceAIndex, int InDiceBIndex)
    {
        if (InDiceAIndex == 1)
            { }
        else if (InDiceAIndex == 2)
            diceA_Rotation.transform.Rotate(new Vector3(90, 90, 0), Space.Self);
        else if (InDiceAIndex == 3)
            diceA_Rotation.transform.Rotate(new Vector3(-90, 0, 0), Space.Self);
        else if (InDiceAIndex == 4)
            diceA_Rotation.transform.Rotate(new Vector3(90, 0, 0), Space.Self);
        else if (InDiceAIndex == 5)
            diceA_Rotation.transform.Rotate(new Vector3(180, -90, 0), Space.Self);
        else if (InDiceAIndex == 6)
            diceA_Rotation.transform.Rotate(new Vector3(180, 0, 0), Space.Self);

        if (InDiceBIndex == 1)
            diceB_Rotation.transform.Rotate(new Vector3(90, 0, 0), Space.Self);
        else if (InDiceBIndex == 2)
            diceB_Rotation.transform.Rotate(new Vector3(180, 0, 0), Space.Self);
        else if (InDiceBIndex == 3)
            diceB_Rotation.transform.Rotate(new Vector3(90, 90, 0), Space.Self);
        else if (InDiceBIndex == 4)
            diceB_Rotation.transform.Rotate(new Vector3(-90, -90, 0), Space.Self);
        else if (InDiceBIndex == 5)
            { }
        else if (InDiceBIndex == 6)
            diceB_Rotation.transform.Rotate(new Vector3(-90, 0, 0), Space.Self);
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

        diceA_Rotation.transform.Rotate(A_RotationVectors[InLoopCount], Space.Self);
        diceB_Rotation.transform.Rotate(B_RotationVectors[InLoopCount], Space.Self);
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
        A_PositionVectors[1] = new Vector3(83.0f, 20.25f, -120.7f);
        A_PositionVectors[2] = new Vector3(81.0f, 20.50f, -121.4f);
        A_PositionVectors[3] = new Vector3(79.0f, 20.75f, -122.1f);
        A_PositionVectors[4] = new Vector3(77.0f, 21.00f, -122.8f);
        A_PositionVectors[5] = new Vector3(75.0f, 21.25f, -123.5f);
        A_PositionVectors[6] = new Vector3(73.0f, 21.50f, -124.2f);
        A_PositionVectors[7] = new Vector3(71.0f, 21.75f, -124.9f);
        A_PositionVectors[8] = new Vector3(69.0f, 22.00f, -125.6f);
        A_PositionVectors[9] = new Vector3(67.0f, 22.25f, -126.3f);

        A_PositionVectors[10] = new Vector3(65.0f, 22.50f, -127.0f);
        A_PositionVectors[11] = new Vector3(63.0f, 22.75f, -127.7f);
        A_PositionVectors[12] = new Vector3(61.0f, 23.00f, -128.4f);
        A_PositionVectors[13] = new Vector3(59.0f, 23.25f, -129.1f);
        A_PositionVectors[14] = new Vector3(57.0f, 23.50f, -129.8f);
        A_PositionVectors[15] = new Vector3(55.0f, 23.75f, -130.5f);
        A_PositionVectors[16] = new Vector3(53.0f, 24.00f, -131.2f);
        A_PositionVectors[17] = new Vector3(51.0f, 24.25f, -131.9f);
        A_PositionVectors[18] = new Vector3(49.0f, 24.50f, -132.6f);
        A_PositionVectors[19] = new Vector3(47.0f, 24.75f, -133.3f);

        A_PositionVectors[20] = new Vector3(45.0f, 25.00f, -134.0f);
        A_PositionVectors[21] = new Vector3(43.0f, 25.25f, -134.7f);
        A_PositionVectors[22] = new Vector3(41.0f, 25.50f, -135.4f);
        A_PositionVectors[23] = new Vector3(39.0f, 25.75f, -136.1f);
        A_PositionVectors[24] = new Vector3(37.0f, 26.00f, -136.8f);
        A_PositionVectors[25] = new Vector3(35.0f, 26.25f, -137.5f);
        A_PositionVectors[26] = new Vector3(33.0f, 26.50f, -138.2f);
        A_PositionVectors[27] = new Vector3(31.0f, 26.75f, -138.9f);
        A_PositionVectors[28] = new Vector3(29.0f, 27.00f, -139.6f);
        A_PositionVectors[29] = new Vector3(27.0f, 27.25f, -140.3f);

        // A주사위 최고 높이 위치 -> 25.0f, 28.0f, -140.0f

        A_PositionVectors[30] = new Vector3(26.35f, 28.0f, -141.00f);
        A_PositionVectors[31] = new Vector3(25.70f, 27.9f, -141.20f);
        A_PositionVectors[32] = new Vector3(25.05f, 27.8f, -142.40f);
        A_PositionVectors[33] = new Vector3(24.40f, 27.7f, -142.60f);
        A_PositionVectors[34] = new Vector3(23.75f, 27.6f, -142.80f);
        A_PositionVectors[35] = new Vector3(23.10f, 27.4f, -143.00f);
        A_PositionVectors[36] = new Vector3(22.45f, 27.2f, -143.30f);
        A_PositionVectors[37] = new Vector3(21.80f, 27.0f, -143.60f);
        A_PositionVectors[38] = new Vector3(21.15f, 26.8f, -143.90f);
        A_PositionVectors[39] = new Vector3(20.50f, 26.6f, -144.30f);

        A_PositionVectors[40] = new Vector3(19.85f, 26.3f, -144.70f);
        A_PositionVectors[41] = new Vector3(18.20f, 26.0f, -145.10f);
        A_PositionVectors[42] = new Vector3(18.55f, 25.7f, -145.50f);
        A_PositionVectors[43] = new Vector3(17.90f, 25.4f, -145.90f);
        A_PositionVectors[44] = new Vector3(17.25f, 25.1f, -146.50f);
        A_PositionVectors[45] = new Vector3(16.60f, 24.7f, -147.00f);
        A_PositionVectors[46] = new Vector3(15.95f, 24.3f, -147.50f);
        A_PositionVectors[47] = new Vector3(15.30f, 23.9f, -148.00f);
        A_PositionVectors[48] = new Vector3(14.65f, 23.5f, -148.50f);
        A_PositionVectors[49] = new Vector3(14.00f, 23.1f, -149.00f);

        A_PositionVectors[50] = new Vector3(13.35f, 22.6f, -149.60f);
        A_PositionVectors[51] = new Vector3(12.70f, 22.1f, -152.20f);
        A_PositionVectors[52] = new Vector3(12.05f, 21.6f, -152.80f);
        A_PositionVectors[53] = new Vector3(11.40f, 21.1f, -153.50f);
        A_PositionVectors[54] = new Vector3(10.75f, 20.6f, -154.20f);
        A_PositionVectors[55] = new Vector3(10.10f, 20.0f, -155.90f);
        A_PositionVectors[56] = new Vector3(09.45f, 19.4f, -156.60f);
        A_PositionVectors[57] = new Vector3(08.80f, 18.8f, -157.30f);
        A_PositionVectors[58] = new Vector3(08.15f, 18.2f, -158.00f);
        A_PositionVectors[59] = new Vector3(07.50f, 17.6f, -158.70f);

        A_PositionVectors[60] = new Vector3(07.00f, 16.9f, -159.40f);
        A_PositionVectors[61] = new Vector3(06.6f, 16.2f, -160.10f);
        A_PositionVectors[62] = new Vector3(06.2f, 15.5f, -160.80f);
        A_PositionVectors[63] = new Vector3(05.8f, 14.8f, -161.50f);
        A_PositionVectors[64] = new Vector3(05.4f, 14.1f, -162.10f);
        A_PositionVectors[65] = new Vector3(05.0f, 13.3f, -162.70f);
        A_PositionVectors[66] = new Vector3(04.6f, 12.6f, -163.20f);
        A_PositionVectors[67] = new Vector3(04.2f, 11.9f, -163.70f);
        A_PositionVectors[68] = new Vector3(03.8f, 11.2f, -164.20f);
        A_PositionVectors[69] = new Vector3(03.4f, 10.5f, -164.70f);

        A_PositionVectors[70] = new Vector3(3.0f, 9.9f, -165.10f);
        A_PositionVectors[71] = new Vector3(2.6f, 9.3f, -165.50f);
        A_PositionVectors[72] = new Vector3(2.2f, 8.7f, -165.90f);
        A_PositionVectors[73] = new Vector3(1.8f, 8.1f, -166.20f);
        A_PositionVectors[74] = new Vector3(1.4f, 7.5f, -166.60f);
        A_PositionVectors[75] = new Vector3(1.0f, 7.0f, -167.00f);
        A_PositionVectors[76] = new Vector3(0.6f, 6.5f, -167.30f);
        A_PositionVectors[77] = new Vector3(0.2f, 6.0f, -167.60f);
        A_PositionVectors[78] = new Vector3(-0.8f, 5.5f, -167.80f);
        A_PositionVectors[79] = new Vector3(-1.4f, 5.0f, -168.00f);

        A_PositionVectors[80] = new Vector3(-1.8f, 4.5f, -168.20f);
        A_PositionVectors[81] = new Vector3(-2.2f, 4.0f, -168.36f);
        A_PositionVectors[82] = new Vector3(-2.6f, 3.5f, -168.52f);
        A_PositionVectors[83] = new Vector3(-3.0f, 3.0f, -168.68f);
        A_PositionVectors[84] = new Vector3(-3.4f, 2.5f, -168.84f);
        A_PositionVectors[85] = new Vector3(-3.8f, 2.0f, -169.00f);

        A_PositionVectors[86] = new Vector3(-4.2f, 1.5f, -169.25f);
        A_PositionVectors[87] = new Vector3(-4.6f, 1.0f, -169.50f);
        A_PositionVectors[88] = new Vector3(-5.0f, 0.5f, -169.75f);
        A_PositionVectors[89] = new Vector3(-5.4f, 0.0f, -170.00f);

        // A주사위 끝 위치 -> -5.0f, 0.0f, -170.0f


        // B주사위 시작 위치 -> 85.0f, 20.0f, -100.0f
        // B주사위 최고 높이 위치 -> 40.0f, 30.0f, -140.0f
        // B주사위 바닥 닿는 위치 -> 20.0f , 10.0f, -155.0f
        // B주사위 끝 위치 -> 5.0f, 0.0f, -165.0f

        B_PositionVectors[0] = new Vector3(85.0f, 20.00f, -100.0f);
        B_PositionVectors[1] = new Vector3(84.0f, 20.25f, -101.0f);
        B_PositionVectors[2] = new Vector3(83.0f, 20.50f, -102.0f);
        B_PositionVectors[3] = new Vector3(82.0f, 20.75f, -103.0f);
        B_PositionVectors[4] = new Vector3(81.0f, 21.00f, -104.0f);
        B_PositionVectors[5] = new Vector3(80.0f, 21.25f, -105.0f);
        B_PositionVectors[6] = new Vector3(79.0f, 21.50f, -106.0f);
        B_PositionVectors[7] = new Vector3(78.0f, 21.75f, -107.0f);
        B_PositionVectors[8] = new Vector3(77.0f, 22.00f, -108.0f);
        B_PositionVectors[9] = new Vector3(76.0f, 22.25f, -109.0f);
        B_PositionVectors[10] = new Vector3(75.0f, 22.50f, -110.0f);
        B_PositionVectors[11] = new Vector3(74.0f, 22.75f, -111.0f);
        B_PositionVectors[12] = new Vector3(73.0f, 23.00f, -112.0f);
        B_PositionVectors[13] = new Vector3(72.0f, 23.25f, -113.0f);
        B_PositionVectors[14] = new Vector3(71.0f, 23.50f, -114.0f);
        B_PositionVectors[15] = new Vector3(70.0f, 23.75f, -115.0f);
        B_PositionVectors[16] = new Vector3(69.0f, 24.00f, -116.0f);
        B_PositionVectors[17] = new Vector3(68.0f, 24.25f, -117.0f);
        B_PositionVectors[18] = new Vector3(67.0f, 24.50f, -118.0f);
        B_PositionVectors[19] = new Vector3(66.0f, 24.75f, -119.0f);
        B_PositionVectors[20] = new Vector3(65.0f, 25.00f, -120.0f);
        B_PositionVectors[21] = new Vector3(64.0f, 25.25f, -121.0f);
        B_PositionVectors[22] = new Vector3(63.0f, 25.50f, -122.0f);
        B_PositionVectors[23] = new Vector3(62.0f, 25.75f, -123.0f);
        B_PositionVectors[24] = new Vector3(61.0f, 26.00f, -124.0f);
        B_PositionVectors[25] = new Vector3(60.0f, 26.25f, -125.0f);
        B_PositionVectors[26] = new Vector3(59.0f, 26.50f, -126.0f);
        B_PositionVectors[27] = new Vector3(58.0f, 26.75f, -127.0f);
        B_PositionVectors[28] = new Vector3(57.0f, 27.00f, -128.0f);
        B_PositionVectors[29] = new Vector3(56.0f, 27.25f, -129.0f);

        B_PositionVectors[30] = new Vector3(54.5f, 28.0f, -130.0f);
        B_PositionVectors[31] = new Vector3(53.0f, 28.25f, -131.0f);
        B_PositionVectors[32] = new Vector3(51.5f, 28.5f, -132.0f);
        B_PositionVectors[33] = new Vector3(50.0f, 28.75f, -133.0f);
        B_PositionVectors[34] = new Vector3(48.5f, 29.0f, -134.0f);
        B_PositionVectors[35] = new Vector3(47.0f, 29.25f, -135.0f);
        B_PositionVectors[36] = new Vector3(45.5f, 29.5f, -136.0f);
        B_PositionVectors[37] = new Vector3(44.0f, 29.75f, -137.0f);
        B_PositionVectors[38] = new Vector3(42.5f, 30.0f, -138.0f);
        B_PositionVectors[39] = new Vector3(41.25f, 30.25f, -139.0f);
        B_PositionVectors[40] = new Vector3(40.0f, 30.5f, -140.0f);

        // B주사위 최고 높이 위치 -> 40.0f, 30.0f, -140.0f

        B_PositionVectors[41] = new Vector3(39.0f, 29.5f, -140.5f);
        B_PositionVectors[42] = new Vector3(38.1f, 28.1f, -141.0f);
        B_PositionVectors[43] = new Vector3(37.5f, 27.5f, -141.5f);
        B_PositionVectors[44] = new Vector3(36.8f, 26.8f, -142.0f);
        B_PositionVectors[45] = new Vector3(36.2f, 26.2f, -142.5f);
        B_PositionVectors[46] = new Vector3(35.5f, 25.5f, -143.0f);
        B_PositionVectors[47] = new Vector3(34.9f, 24.9f, -143.5f);
        B_PositionVectors[48] = new Vector3(34.3f, 24.3f, -144.0f);
        B_PositionVectors[49] = new Vector3(33.6f, 23.6f, -144.5f);
        B_PositionVectors[50] = new Vector3(33.0f, 23.0f, -145.0f);
        B_PositionVectors[51] = new Vector3(32.3f, 22.3f, -145.5f);
        B_PositionVectors[52] = new Vector3(31.7f, 21.7f, -146.0f);
        B_PositionVectors[53] = new Vector3(31.1f, 21.1f, -146.5f);
        B_PositionVectors[54] = new Vector3(30.4f, 20.4f, -147.0f);
        B_PositionVectors[55] = new Vector3(29.8f, 19.8f, -147.5f);
        B_PositionVectors[56] = new Vector3(29.1f, 19.1f, -148.0f);
        B_PositionVectors[57] = new Vector3(28.4f, 18.4f, -148.5f);
        B_PositionVectors[58] = new Vector3(27.8f, 17.8f, -149.0f);
        B_PositionVectors[59] = new Vector3(27.1f, 17.1f, -149.5f);
        B_PositionVectors[60] = new Vector3(26.5f, 16.5f, -150.0f);
        B_PositionVectors[61] = new Vector3(25.8f, 15.8f, -150.5f);
        B_PositionVectors[62] = new Vector3(25.2f, 15.2f, -151.0f);
        B_PositionVectors[63] = new Vector3(24.5f, 14.5f, -151.5f);
        B_PositionVectors[64] = new Vector3(23.9f, 13.9f, -152.0f);
        B_PositionVectors[65] = new Vector3(23.2f, 13.2f, -152.5f);
        B_PositionVectors[66] = new Vector3(22.6f, 12.6f, -153.0f);
        B_PositionVectors[67] = new Vector3(21.9f, 11.9f, -153.5f);
        B_PositionVectors[68] = new Vector3(21.3f, 11.3f, -154.0f);
        B_PositionVectors[69] = new Vector3(20.6f, 10.7f, -154.5f);
        B_PositionVectors[70] = new Vector3(20.0f, 10.0f, -155.0f);

        // B주사위 바닥 닿는 위치 -> 20.0f , 10.0f, -155.0f

        B_PositionVectors[71] = new Vector3(18.5f, 9.5f, -155.5f);
        B_PositionVectors[72] = new Vector3(17.75f, 9.0f, -156.0f);
        B_PositionVectors[73] = new Vector3(17.00f, 8.5f, -156.5f);
        B_PositionVectors[74] = new Vector3(16.25f, 8.0f, -157.0f);
        B_PositionVectors[75] = new Vector3(15.5f, 7.5f, -157.5f);
        B_PositionVectors[76] = new Vector3(14.75f, 7.0f, -158.0f);
        B_PositionVectors[77] = new Vector3(14.00f, 6.5f, -158.5f);
        B_PositionVectors[78] = new Vector3(13.25f, 6.0f, -159.0f);
        B_PositionVectors[79] = new Vector3(12.5f, 5.5f, -159.5f);
        B_PositionVectors[80] = new Vector3(11.75f, 5.0f, -160.0f);
        B_PositionVectors[81] = new Vector3(11.0f, 4.5f, -160.5f);
        B_PositionVectors[82] = new Vector3(10.25f, 4.0f, -161.0f);
        B_PositionVectors[83] = new Vector3(9.5f, 3.5f, -161.5f);
        B_PositionVectors[84] = new Vector3(8.75f, 3.0f, -162.0f);
        B_PositionVectors[85] = new Vector3(8.00f, 2.5f, -162.5f);
        B_PositionVectors[86] = new Vector3(7.25f, 2.0f, -163.0f);
        B_PositionVectors[87] = new Vector3(6.5f, 1.5f, -163.5f);
        B_PositionVectors[88] = new Vector3(5.75f, 1.0f, -164.0f);
        B_PositionVectors[89] = new Vector3(5.0f, 0.5f, -164.5f);

        // B주사위 끝 위치 -> 5.0f, 0.0f, -165.0f
    }

    /*
    * SetDiceRotation
    * 
    *  - 최초 Dice 로테이션을 세팅 해줍니다...
    */
    private void SetDiceRotation()
    {
        int A_RotateBuffer = 123;
        int B_RotateBuffer = 100;

        for (int i = 0; i < 90; ++i)
           A_RotationVectors[i] = new Vector3((A_RotateBuffer - i) * 0.03f, (A_RotateBuffer - i) * 0.045f, (A_RotateBuffer - i) * 0.04f);
      
       for (int i = 0; i < 90; ++i)
           B_RotationVectors[i] = new Vector3((B_RotateBuffer - i) * 0.02f, (B_RotateBuffer - i) * 0.06f, (B_RotateBuffer - i) * 0.03f);
    }
}
