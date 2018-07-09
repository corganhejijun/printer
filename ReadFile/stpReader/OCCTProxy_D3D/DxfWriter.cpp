#include "DxfWriter.h"

DxfWriter::DxfWriter(const wchar_t* fileName) {
    char* name = NULL;
    wtochar(&name, fileName);
    dw = dxf.out(name, DL_Codes::AC1015);
    free(name);
}

DxfWriter::~DxfWriter() {

}

void DxfWriter::writeHead() {
    dxf.writeHeader(*dw);
    dw->sectionEnd();
    dw->sectionTables();
    dxf.writeVPort(*dw);

    dw->tableLineTypes(1);
    dxf.writeLineType(*dw, DL_LineTypeData("CONTINUOUS", 0));
    dxf.writeLineType(*dw, DL_LineTypeData("BYLAYER", 0));
    dxf.writeLineType(*dw, DL_LineTypeData("BYBLOCK", 0));
    dw->tableEnd();

    dw->tableLayers(1);
    dxf.writeLayer(
        *dw,
        DL_LayerData("0", 0),
        DL_Attributes("", 1, 0x00ff0000, 15, "CONTINUOUS")
    );
    dw->tableEnd();

    dxf.writeView(*dw);
    dxf.writeUcs(*dw);

    dw->tableAppid(1);
    dxf.writeAppid(*dw, "ACAD");
    dw->tableEnd();

    dxf.writeDimStyle(*dw, 2.5, 0.625, 0.625, 0.625, 2.5);

    dxf.writeBlockRecord(*dw);
    dw->tableEnd();
    dw->sectionEnd();

    dw->sectionBlocks();
    dxf.writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
    dxf.writeEndBlock(*dw, "*Model_Space");
    dxf.writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
    dxf.writeEndBlock(*dw, "*Paper_Space");
    dxf.writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
    dxf.writeEndBlock(*dw, "*Paper_Space0");
    dw->sectionEnd();

    dw->sectionEntities();
}

void DxfWriter::writeEnd() {
    dw->sectionEnd();
    dxf.writeObjects(*dw);
    dxf.writeObjectsEnd(*dw);
    dw->dxfEOF();
    dw->close();
    delete dw;
}

void DxfWriter::writeLine(double startX, double startY, double startZ, double endX, double endY, double endZ) {
    dw->dxfString(0, "LINE");
    dw->dxfString(10, std::to_string(startX));
    dw->dxfString(20, std::to_string(startY));
    dw->dxfString(30, std::to_string(startZ));
    dw->dxfString(11, std::to_string(endX));
    dw->dxfString(21, std::to_string(endY));
}

void DxfWriter::writeCircle(double x, double y, double z, double r, int normal) {
    /*
    dw->dxfString(0, "CIRCLE");
    dw->dxfString(10, std::to_string(x));
    dw->dxfString(20, std::to_string(y));
    dw->dxfString(30, std::to_string(z));
    dw->dxfString(40, std::to_string(r));
    if (normal < 0) {
        dw->dxfString(230, std::to_string(-1.0));
    }
    */
    DL_Attributes attributes("0", 256, 0x00ffff00, 5, "BYLAYER");
    DL_CircleData circleData(x, y, z, r);
    dxf.writeCircle(*dw, circleData, attributes);
}

void DxfWriter::writeArc(double x, double y, double z, double r, double startAngle, double endAngle, int normal) {
    /*
    dw->dxfString(0, "ARC");
    dw->dxfString(10, std::to_string(x));
    dw->dxfString(20, std::to_string(y));
    dw->dxfString(30, std::to_string(z));
    dw->dxfString(40, std::to_string(r));
    dw->dxfString(50, std::to_string(startAngle));
    dw->dxfString(51, std::to_string(endAngle));
    if (normal < 0) {
        dw->dxfString(230, std::to_string(-1.0));
    }
    */
    DL_Attributes attributes("0", 256, 0x00ffff00, 5, "BYLAYER");
    DL_ArcData arcData(x, y, z, r, startAngle, endAngle);
    dxf.writeArc(*dw, arcData, attributes);
}

int DxfWriter::wtochar(char **ppDest, const wchar_t *pSrc)
{
    int len = 0;
    int ret = 0;

    len = wcslen(pSrc) + 1;

    if (len <= 1) return 0;

    /*sizeof(wchar_t) = 4 */
    *ppDest = (char*)malloc(len * sizeof(wchar_t));

    /*这里的第三个长度参数，应为字节长度，即宽字符长度 * 4 */
    ret = wcstombs(*ppDest, pSrc, len * sizeof(wchar_t));

    return ret;
}