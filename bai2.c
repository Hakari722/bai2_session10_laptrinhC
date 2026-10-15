#include <stdio.h>
#include <string.h>

#define MAX_VE 50

struct VeSuKien {
    int maVe;
    char tenSuKien[50];
    char loaiVe[20];
    float giaGoc;
    int isEarlyBird;
    int daCheckIn;
};

int timVeTheoMa(const struct VeSuKien ds[], int soLuong, int maVeCanTim) {
    for (int i = 0; i < soLuong; i++) {
        if (ds[i].maVe == maVeCanTim) {
            return i;
        }
    }
    return -1;
}

int phatHanhVe(struct VeSuKien ds[], int soLuong, struct VeSuKien veMoi) {
    if (soLuong >= MAX_VE) {
        printf("[LỖI]: Sức chứa hệ thống đã đầy (%d vé), không thể phát hành thêm!\n", MAX_VE);
        return soLuong;
    }
    
    if (veMoi.isEarlyBird == 1) {
        veMoi.giaGoc = veMoi.giaGoc * 0.85f;
    }
    veMoi.daCheckIn = 0;

    ds[soLuong] = veMoi;
    printf("[THÀNH CÔNG]: Đã phát hành vé mã %d (%s - %s).\n", veMoi.maVe, veMoi.tenSuKien, veMoi.loaiVe);
    return soLuong + 1;
}

void checkInQR(struct VeSuKien ds[], int soLuong, int maVe) {
    int index = timVeTheoMa(ds, soLuong, maVe);
    if (index == -1) {
        printf("[LỖI CHECK-IN]: Không tìm thấy mã vé %d hợp lệ trong hệ thống!\n", maVe);
        return;
    }

    if (ds[index].daCheckIn == 1) {
        printf("[CẢNH BÁO GIAN LẬN]: Vé mã %d của hành khách đã được check-in trước đó! Từ chối qua cổng.\n", maVe);
    } else {
        ds[index].daCheckIn = 1;
        printf("[CHECK-IN THÀNH CÔNG]: Vé mã %d được phép qua cổng cho khu vực %s.\n", maVe, ds[index].loaiVe);
    }
}

int huyVe(struct VeSuKien ds[], int soLuong, int maVeCanHuy) {
    int index = timVeTheoMa(ds, soLuong, maVeCanHuy);
    if (index == -1) {
        printf("[LỖI HỦY VÉ]: Không tìm thấy mã vé %d để hủy!\n", maVeCanHuy);
        return soLuong;
    }

    for (int j = index; j < soLuong - 1; j++) {
        ds[j] = ds[j + 1];
    }
    printf("[THÀNH CÔNG]: Đã hủy thành công vé có mã %d ra khỏi hệ thống.\n", maVeCanHuy);
    return soLuong - 1;
}

void hienThiDanhSach(const struct VeSuKien ds[], int soLuong) {
    if (soLuong == 0) {
        printf("[THÔNG BÁO]: Danh sách vé hiện đang rỗng!\n");
        return;
    }

    printf("\n======================================= DANH SÁCH VÉ SỰ KIỆN =======================================\n");
    printf("%-4s | %-8s | %-25s | %-10s | %-15s | %-12s | %-10s\n", 
           "STT", "MÃ VÉ", "TÊN SỰ KIỆN", "HẠNG VÉ", "GIÁ THANH TOÁN", "EARLY BIRD", "CHECK-IN");
    printf("----------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < soLuong; i++) {
        char earlyStr[5] = "Không";
        if (ds[i].isEarlyBird == 1) {
            strcpy(earlyStr, "Có");
        }

        char checkStr[15] = "Chưa";
        if (ds[i].daCheckIn == 1) {
            strcpy(checkStr, "Đã vào cổng");
        }

        printf("%-4d | %-8d | %-25s | %-10s | %-15.0f | %-12s | %-10s\n",
               i + 1,
               ds[i].maVe,
               ds[i].tenSuKien,
               ds[i].loaiVe,
               ds[i].giaGoc,
               earlyStr,
               checkStr);
    }
    printf("================================================================================================----\n");
}

int main() {
    struct VeSuKien danhSach[MAX_VE];
    int soLuongVe = 0;

    printf("=== DEMO MODULE QUẢN LÝ & SOÁT VÉ TICKETBOX ===\n\n");

    struct VeSuKien v1 = {1001, "Live Concert VIBE 2025", "VIP", 2000000.0f, 1, 0};
    struct VeSuKien v2 = {1002, "Live Concert VIBE 2025", "Zone A", 1200000.0f, 0, 0};
    struct VeSuKien v3 = {1003, "Live Concert VIBE 2025", "GA", 800000.0f, 1, 0};

    soLuongVe = phatHanhVe(danhSach, soLuongVe, v1);
    soLuongVe = phatHanhVe(danhSach, soLuongVe, v2);
    soLuongVe = phatHanhVe(danhSach, soLuongVe, v3);

    hienThiDanhSach(danhSach, soLuongVe);

    printf("\n--- 1. THỬ NGHIỆM PHÁT HÀNH THÊM VÉ MỚI ---\n");
    struct VeSuKien v4 = {1004, "Live Concert VIBE 2025", "VIP", 2500000.0f, 0, 0};
    soLuongVe = phatHanhVe(danhSach, soLuongVe, v4);

    printf("\n--- 2. THỰC HIỆN CHECK-IN QR THÀNH CÔNG VÉ 1001 ---\n");
    checkInQR(danhSach, soLuongVe, 1001);

    printf("\n--- 3. THỬ NGHIỆM QUÉT LẠI MÃ VÉ 1001 (DUPLICATE QR SCAN) ---\n");
    checkInQR(danhSach, soLuongVe, 1001);

    printf("\n--- 4. THỰC HIỆN HỦY VÉ MÃ 1002 ---\n");
    soLuongVe = huyVe(danhSach, soLuongVe, 1002);

    printf("\n--- 5. THỬ NGHIỆM CHECK-IN MÃ VÉ KHÔNG TỒN TẠI (9999) ---\n");
    checkInQR(danhSach, soLuongVe, 9999);

    hienThiDanhSach(danhSach, soLuongVe);

    return 0;
}