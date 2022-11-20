// #ifndef HAS_TESTDATA
// #define HAS_TESTDATA


// /// @brief Data has been taken using stellarium to observe a range of objects at
// /// different times of day.
// const struct testData {
    const int numRefs = 9;
    const String refName[numRefs] = { "HIP 66958 ", "HIP 66958 ", "HIP 66958 ", "HIP 66958 ", "Polaris Australis", "Polaris Australis", "Jupiter", "Jupiter", "Jupiter"};
    const String refSkyRaStr[numRefs] = {  "13:44:20",  "13:44:20",  "13:44:20",  "13:44:20",  "21:28:07",  "21:28:06",  "00:15:41",  "00:15:39",  "00:16:45"};
    const String refSkyDecStr[numRefs] = { "+45*24:34", "+45*24:34", "+45*24:34", "+45*24:34", "-88*51:57", "-88*51:57", "-00*04:01", "-00*04:14", "+00*02:52"};
    const String refBaseLhaStr[numRefs] = { " 23:59:09", " 23:50:18", " 00:06:17", " 03:12:32", " 13:36:17", " 07:50:36", " 05:03:01", " 06:41:38", " 01:25:48"};
    const String refSidTme[numRefs] = { "13:43:30", "13:34:39", "13:50:38", "16:56:52", "11:04:24", "05:18:43", "05:18:43", "06:57:17", "01:42:32"};
    const String inAzStr[] = { " +00:09:04", " +01:42:47", " +358:53:18", " +328:24:09", " +179:25:22", " +181:15:43", "+278:47:16", " +263:31:27", " +327:21:40"};
    const String inAltStr[] = { "+06:48:25", "+06:46:43", "+06:47:42", "-03:47:06", "+36:44:47", "+37:15:01", "+11:15:20", "-08:09:50", "+47:18:54"};
    
    const double refSkyRa[numRefs] = { 206.08, 206.08, 206.08, 206.08, 322.03, 322.03, 03.92, 03.91, 04.19};
    const double refSkyDec[numRefs] = { 45.41, 45.41, 45.41, 45.41, -88.87, -88.87, -00.07, -00.07, 00.05};
    const double refBaseLha[numRefs] = { 359.79, 357.58, 01.57, 48.13, 204.07, 117.65, 75.76, 100.41, 21.45};
    const double refBaseDec[numRefs] = { 45.41, 45.41, 45.41, 45.41, -88.87, -88.87, -00.07, -00.07, 00.05};
    const double refAltazAz[numRefs] = { 00.15, 01.71, 358.89, 328.40, 179.42, 181.26, 278.79, 263.52, 00.36};
    const double refAltazAlt[numRefs] = { 06.81, 06.78, 06.80, -03.79, 36.75, 37.25, 11.26, -08.16, 47.32};
    const double refSiderealTime[numRefs] = { 205.87, 203.66, 207.66, 254.22, 166.10, 79.68, 79.68, 104.32, 25.63};
    
//     // These values are inferred from above data, and were calculated manually
//     const double refMtrLha[numRefs] = { 359.79, 357.58, 01.57, 48.13, 204.07, 117.65, 75.76, 100.41, 21.45};
//     const double refMtrDec[] = { 45.41, 45.41, 45.41, 45.41, -88.87, -88.87, -00.07, -00.07, 00.05};
//     const double refHomeLha[] = { 00.00, 00.00, 00.00, 00.00, 00.00, 00.00, 00.00, 00.00, 00.00};
//     const double refHomeDec[] = { 47.00, 47.00, 47.00, 47.00, 47.00, 47.00, 47.00, 47.00, 47.00};
    
//     const double diffang1[] = { 354.00, 358.43};
//     const double diffang2[] = { 358.00, 2.43};
//     const double diffang3[] = { 2.00, 6.43};
//     const double diffAns = 4.43;
//     const double diffAns2 = 355.57;
// } tst;
// #endif // HAS_TESTDATA