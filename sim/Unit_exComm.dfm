inherited Task_exComm: TTask_exComm
  Left = 667
  Top = 170
  Width = 835
  Height = 528
  Caption = #22806#37096#25509#21475
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 12
  inherited Panel1: TPanel
    Width = 819
  end
  inherited Memo_Info: TMemo
    Top = 179
    Width = 819
    Height = 311
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 41
    Width = 819
    Height = 138
    Align = alTop
    Caption = #25509#21475#21442#25968
    TabOrder = 2
    object Bevel2: TBevel
      Left = 2
      Top = 14
      Width = 815
      Height = 37
      Align = alTop
      Shape = bsBottomLine
    end
    object Bevel1: TBevel
      Left = 2
      Top = 51
      Width = 815
      Height = 37
      Align = alTop
      Shape = bsBottomLine
    end
    object Label1: TLabel
      Left = 16
      Top = 24
      Width = 42
      Height = 12
      Caption = 'RS232'#65306
    end
    object Label3: TLabel
      Left = 449
      Top = 24
      Width = 60
      Height = 12
      Caption = #26381#21153#31471#21475#65306
    end
    object Label4: TLabel
      Left = 15
      Top = 62
      Width = 42
      Height = 12
      Caption = #32418' '#22806#65306
    end
    object Label6: TLabel
      Left = 448
      Top = 62
      Width = 60
      Height = 12
      Caption = #26381#21153#31471#21475#65306
    end
    object lbl1: TLabel
      Left = 16
      Top = 104
      Width = 36
      Height = 12
      Caption = #22320#22336#65306
    end
    object lbl2: TLabel
      Left = 216
      Top = 104
      Width = 36
      Height = 12
      Caption = #31471#21475#65306
    end
    object Label2: TLabel
      Left = 80
      Top = 23
      Width = 60
      Height = 12
      Caption = #36733#27874#31471#21475#65306
    end
    object Label5: TLabel
      Left = 216
      Top = 23
      Width = 48
      Height = 12
      Caption = #27874#29305#29575#65306
      Font.Charset = GB2312_CHARSET
      Font.Color = clNavy
      Font.Height = -12
      Font.Name = #23435#20307
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 344
      Top = 23
      Width = 48
      Height = 12
      Caption = #26657#39564#20301#65306
      Font.Charset = GB2312_CHARSET
      Font.Color = clNavy
      Font.Height = -12
      Font.Name = #23435#20307
      Font.Style = []
      ParentFont = False
    end
    object CheckBox_RS232: TCheckBox
      Left = 59
      Top = 20
      Width = 78
      Height = 17
      Caption = #30495#23454#20018#21475
      TabOrder = 0
    end
    object Edit_RS232_Port: TEdit
      Left = 511
      Top = 20
      Width = 41
      Height = 20
      ImeName = #24494#36719#25340#38899#36755#20837#27861' 2007'
      TabOrder = 1
      Text = '4001'
    end
    object BitBtn_RS232_Connect: TBitBtn
      Left = 556
      Top = 16
      Width = 75
      Height = 25
      Caption = #21551#21160
      TabOrder = 2
      OnClick = BitBtn_RS232_ConnectClick
    end
    object BitBtn_RS232_Discon: TBitBtn
      Left = 636
      Top = 16
      Width = 75
      Height = 25
      Caption = #20572#27490
      TabOrder = 3
      OnClick = BitBtn_RS232_DisconClick
    end
    object CheckBox1: TCheckBox
      Left = 58
      Top = 58
      Width = 78
      Height = 17
      Caption = #30495#23454#20018#21475
      TabOrder = 4
    end
    object Edit_Infra_Port: TEdit
      Left = 510
      Top = 58
      Width = 41
      Height = 20
      ImeName = #24494#36719#25340#38899#36755#20837#27861' 2007'
      TabOrder = 5
      Text = '4002'
    end
    object BitBtn_Infra_Connect: TBitBtn
      Left = 555
      Top = 54
      Width = 75
      Height = 25
      Caption = #21551#21160
      TabOrder = 6
    end
    object BitBtn_Infra_Discon: TBitBtn
      Left = 635
      Top = 54
      Width = 75
      Height = 25
      Caption = #20572#27490
      TabOrder = 7
    end
    object btn_remote_connect: TButton
      Left = 424
      Top = 104
      Width = 75
      Height = 25
      Caption = #21551#21160
      TabOrder = 8
      OnClick = btn_remote_connectClick
    end
    object cbb_addr: TComboBox
      Left = 48
      Top = 104
      Width = 145
      Height = 20
      ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 12
      TabOrder = 9
      Text = '127.0.0.1'
      Items.Strings = (
        '127.0.0.1'
        '219.147.26.62'
        '120.198.68.205')
    end
    object cbb_port: TComboBox
      Left = 248
      Top = 104
      Width = 145
      Height = 20
      ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 12
      TabOrder = 10
      Text = '4001'
      Items.Strings = (
        '4001'
        '5024'
        '54321')
    end
    object cbb_rs232: TComboBox
      Left = 128
      Top = 19
      Width = 65
      Height = 20
      Style = csDropDownList
      Font.Charset = GB2312_CHARSET
      Font.Color = clNavy
      Font.Height = -12
      Font.Name = #23435#20307
      Font.Style = []
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 12
      ItemIndex = 0
      ParentFont = False
      TabOrder = 11
      Text = 'COM1'
      Items.Strings = (
        'COM1'
        'COM2'
        'COM3'
        'COM4'
        'COM5'
        'COM6'
        'COM7'
        'COM8'
        'COM9'
        'COM10'
        'COM11'
        'COM12'
        'COM13'
        'COM14'
        'COM15'
        'COM16'
        'COM17'
        'COM18'
        'COM19'
        'COM20'
        'COM21')
    end
    object cbb_BaudRate: TComboBox
      Left = 264
      Top = 19
      Width = 65
      Height = 20
      Style = csDropDownList
      Font.Charset = GB2312_CHARSET
      Font.Color = clNavy
      Font.Height = -12
      Font.Name = #23435#20307
      Font.Style = []
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 12
      ItemIndex = 5
      ParentFont = False
      TabOrder = 12
      Text = '115200'
      Items.Strings = (
        '1200'
        '9600'
        '19200'
        '38400'
        '57600'
        '115200')
    end
    object cbb_Parity: TComboBox
      Left = 392
      Top = 19
      Width = 65
      Height = 20
      Style = csDropDownList
      Font.Charset = GB2312_CHARSET
      Font.Color = clNavy
      Font.Height = -12
      Font.Name = #23435#20307
      Font.Style = []
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 12
      ItemIndex = 1
      ParentFont = False
      TabOrder = 13
      Text = #20598#26657#39564
      Items.Strings = (
        #26080#26657#39564
        #20598#26657#39564
        #22855#26657#39564
        '')
    end
  end
  object mscm_rs232: TMSComm
    Left = 648
    Top = 272
    Width = 32
    Height = 32
    OnComm = mscm_rs232Comm
    ControlData = {
      2143341208000000ED030000ED03000001568A64000006000000010000040000
      00020100802500000000080000000000000000003F00000011000000}
  end
  object ServerSocket_RS232: TServerSocket
    Active = False
    Port = 0
    ServerType = stNonBlocking
    OnListen = ServerSocket_RS232Listen
    OnAccept = ServerSocket_RS232Accept
    OnClientRead = ServerSocket_RS232ClientRead
    OnClientWrite = ServerSocket_RS232ClientWrite
    Left = 472
    Top = 256
  end
  object clntsckt_remote: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = clntsckt_remoteConnect
    OnRead = clntsckt_remoteRead
    Left = 544
    Top = 256
  end
  object tmr1: TTimer
    Interval = 300000
    OnTimer = tmr1Timer
    Left = 352
    Top = 304
  end
end
