all:
	meson debug --cross-file "kiso/boards/CommonGateway/meson_config_stm32l4_gcc8.ini"
	cd debug && meson configure -Db_pch=false -Db_staticpic=false
	cd debug && ninja hex
