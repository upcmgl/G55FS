inherited Task_readPortService: TTask_readPortService
  Left = 416
  Top = 239
  Width = 1004
  Height = 488
  Caption = #25220#35835#31471#21475#26381#21153
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 12
  inherited Panel1: TPanel
    Width = 988
  end
  inherited Memo_Info: TMemo
    Top = 89
    Width = 988
    Height = 361
  end
  object pnl2: TPanel
    Left = 0
    Top = 41
    Width = 988
    Height = 48
    Align = alTop
    TabOrder = 2
    object btn1: TButton
      Left = 144
      Top = 11
      Width = 75
      Height = 25
      Caption = #36830#25509
      TabOrder = 0
      OnClick = btn1Click
    end
    object cbb_comm_485: TComboBox
      Left = 408
      Top = 11
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
      ItemIndex = 2
      ParentFont = False
      TabOrder = 1
      Text = 'COM3'
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
    object btn2: TButton
      Left = 504
      Top = 9
      Width = 75
      Height = 25
      Caption = #25171#24320
      TabOrder = 2
      OnClick = btn2Click
    end
    object rbnet: TRadioButton
      Left = 8
      Top = 13
      Width = 65
      Height = 17
      Caption = #32593#32476
      Checked = True
      TabOrder = 3
      TabStop = True
    end
    object rbrs232: TRadioButton
      Left = 336
      Top = 13
      Width = 65
      Height = 17
      Caption = #20018#21475
      TabOrder = 4
    end
    object edtwangkou: TEdit
      Left = 56
      Top = 12
      Width = 81
      Height = 20
      ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      TabOrder = 5
      Text = '4002'
    end
  end
  object mscm_comm_485: TMSComm
    Left = 456
    Top = 128
    Width = 32
    Height = 32
    OnComm = mscm_comm_485Comm
    ControlData = {
      2143341208000000ED030000ED03000001568A64000006000000010000040000
      00020100802500000000080000000000000000003F00000011000000}
  end
  object clntsckt_RS485_1: TClientSocket
    Active = False
    Address = '127.0.0.1'
    ClientType = ctNonBlocking
    Host = '127.0.0.1'
    Port = 4002
    OnConnect = clntsckt_RS485_1Connect
    OnRead = clntsckt_RS485_1Read
    Left = 344
    Top = 88
  end
end
