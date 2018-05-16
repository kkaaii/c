#ifndef	_FW_COMMIT_H
#define	_FW_COMMIT_H

enum {
	/*
	** 000b - Downloaded image replaces the image specified by the FS field.
	** This image is not activated.
	*/
	eCA_COMMIT_NO_ACTIVATION = 0,

	/*
	** 001b - Downloaded image replaces the image specified by the FS field.
	** This image is activated at the next reset.
	*/
	eCA_COMMIT_AND_ACTIVATE_WITH_RESET = 1,

	/*
	** 010b - The image specified by the FS field is activated at the next reset.
	*/
	eCA_ACTIVATE_WITH_RESET = 2,

	/*
	** 011b - The image specified by the FS field is requested to be activated
	** immediately without reset.
	*/
	eCA_ACTIVATE_IMMEDIATELY = 3
};

#endif	/* _GET_LOGPAGE_H */

