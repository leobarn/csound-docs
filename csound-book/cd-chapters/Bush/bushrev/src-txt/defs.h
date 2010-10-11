/* defs.h */
/* macros for sco operations */

/* Csound does not use p0 */

#define F_NUM		1
#define F_STA		2
#define F_GEN		3
#define F_SIZE		4

#define I1PF "i", "sta", "dur", "pch1", "pc1", "wob1", "pdf1", "wid1", "atk1", "dec1", "pdB1", "iloc1", "eloc1", "pch2", "pc2", "wob2", "pdf2", "wid2", "atk2", "dec2", "pdB2", "iloc2", "eloc2", "rch"

#define II_I		1
#define II_STA		2
#define II_DUR		3

#define I1_PCH1		4
#define I1_PC1		5
#define I1_WOB1		6
#define I1_PDF1		7
#define I1_WID1		8
#define I1_ATK1		9
#define I1_DEC1		10
#define I1_PDB1		11
#define I1_ILOC1	12
#define I1_ELOC1	13

#define I1_PCH2		14
#define I1_PC2		15
#define I1_WOB2		16
#define I1_PDF2		17
#define I1_WID2		18
#define I1_ATK2		19
#define I1_DEC2		20
#define I1_PDB2		21
#define I1_ILOC2	22
#define I1_ELOC2	23

#define I1_RCH		24
#define NUM_I1PFIELDS 24

#define I2PF "i", "sta", "dur", "freq1", "wid1", "pcd1", "atk1", "dec1", "pdB1", "dist1", "azi1", "freq2", "wid2", "pcd2", "atk2", "dec2", "pdB2", "dist2", "azi2"

#define I2_FREQ1	4
#define I2_WID1		5
#define I2_PDF1		6
#define I2_ATK1		7
#define I2_DEC1		8
#define I2_PDB1		9
#define I2_DIST1	10
#define I2_AZI1		11

#define I2_FREQ2	12
#define I2_WID2		13
#define I2_PDF2		14
#define I2_ATK2		15
#define I2_DEC2		16
#define I2_PDB2		17
#define I2_DIST2	18
#define I2_AZI2		19

#define NUM_I2PFIELDS 19

#define I4PF "i", "sta", "dur", "pch1", "pc1", "wob1", "wid1", "pdf1", "atk1", "dec1", "pdB1", "dist1", "azi1", "pch2", "pc2", "wob2", "wid2", "pdf2", "atk2", "dec2", "pdB2", "dist2", "azi2"

#define I4_PCH1		4
#define I4_PC1		5
#define I4_WOB1		6
#define I4_WID1		7
#define I4_PDF1		8
#define I4_ATK1		9
#define I4_DEC1		10
#define I4_PDB1		11
#define I4_DIST1	12
#define I4_AZI1		13

#define I4_PCH2		14
#define I4_PC2		15
#define I4_WOB2		16
#define I4_WID2		17
#define I4_PDF2		18
#define I4_ATK2		19
#define I4_DEC2		20
#define I4_PDB2		21
#define I4_DIST2	22
#define I4_AZI2		23

#define NUM_I4PFIELDS 23

#define DEF_ASCFFILE "ascf.adsyn"
#define DEF_FSCOFILE "score.sco"
#define DEF_MRSCOFILE "score.mrsco"

#define MOST_PFIELDS 22
