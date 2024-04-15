CREATE TABLE IF NOT EXISTS `TestAccount` (
  `account` decimal(6,0) NOT NULL COMMENT '帳號',
  `bsamt` decimal(4,0) NOT NULL COMMENT '委託額度(萬)',
  PRIMARY KEY (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='客戶基本資料';


CREATE TABLE IF NOT EXISTS `TestStock` (
  `account` decimal(6,0) NOT NULL COMMENT '帳號',
  `stock` varchar(6) NOT NULL COMMENT '股票代號',
  `qty` decimal(9,0) NOT NULL COMMENT '集保股數',
  PRIMARY KEY (`account`, `stock`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='客戶庫存資料';