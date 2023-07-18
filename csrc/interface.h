#define PM_CN_MAX 512
#define PM_CN_SHIFT 3
#define PM_DEPTH_MAX (1 << PM_CN_SHIFT)
#define PM_8U 0
#define PM_8S 1
#define PM_16U 2
#define PM_16S 3
#define PM_32S 4
#define PM_32F 5
#define PM_64F 6
#define PM_USRTYPE1 7

#define PM_MAT_DEPTH_MASK (PM_DEPTH_MAX - 1)
#define PM_MAT_DEPTH(flags) ((flags) & PM_MAT_DEPTH_MASK)
#define PM_MAKETYPE(depth, cn) (PM_MAT_DEPTH(depth) + (((cn)-1) << PM_CN_SHIFT))

#define PM_8UC1 PM_MAKETYPE(PM_8U,1)
#define PM_8UC3 PM_MAKETYPE(PM_8U,3)
#define PM_32SC3 PM_MAKETYPE(PM_32S,3)
#define PM_64FC4 PM_MAKETYPE(PM_64F,4)